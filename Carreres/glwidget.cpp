#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"
//#include <QGlobal.h>
#include <QTime>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    setFocusPolicy( Qt::StrongFocus );
    esc = new escena();
    cameraActual = this->esc->camaraPanoramica;

    third = false;
    index = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    a = 50.0;
    h = 50.0;
    p = 50.0;

    modScalat = Ortho(-25, 25,-25, 25,-25, 25);

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

    program = 0;
    flatshader = 0;
    gouraudshader = 0;
    phongshader = 0;

}


GLWidget::~GLWidget()
{
    makeCurrent();
}

//  Metode per a carregar de fitxers el vertex i el fragment shader
void GLWidget::InitShader(const char* vShaderFile, const char* fShaderFile, string type)
{

    struct Shader {
        const char*  filename;
        GLenum       type;
        GLchar*      source;
    }  shaders[2] = {
    { vShaderFile, GL_VERTEX_SHADER, NULL },
    { fShaderFile, GL_FRAGMENT_SHADER, NULL }
};


    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);
    // Es llegeixen els dos shaders: el vertex i el fragment shader
    for ( int i = 0; i < 2; ++i ) {
        Shader& s = shaders[i];
        s.source = Common::readShaderSource( s.filename );
        if ( shaders[i].source == NULL ) {
            std::cerr << "Failed to read " << s.filename << std::endl;
            exit( EXIT_FAILURE );
        }
    }

    vshader->compileSourceCode(shaders[0].source);
    fshader->compileSourceCode(shaders[1].source);

    // Cargamos cada uno de los shaders segun el tipo
    if(type == "flat"){
        flatshader = new QGLShaderProgram(this);
        flatshader->addShader(vshader);
        flatshader->addShader(fshader);
        flatshader->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
        //flatshader->bindAttributeLocation("vNormal", PROGRAM_COLOR_ATTRIBUTE);
        flatshader->link();
    }else if(type == "gouraud"){
        gouraudshader = new QGLShaderProgram(this);
        gouraudshader->addShader(vshader);
        gouraudshader->addShader(fshader);
        gouraudshader->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
        //gouraudshader->bindAttributeLocation("vNormal", PROGRAM_COLOR_ATTRIBUTE);
        gouraudshader->link();
    }else if(type == "phong"){
        phongshader = new QGLShaderProgram(this);
        phongshader->addShader(vshader);
        phongshader->addShader(fshader);
        phongshader->bindAttributeLocation("vPosition", PROGRAM_VERTEX_ATTRIBUTE);
        //phongshader->bindAttributeLocation("vNormal", PROGRAM_COLOR_ATTRIBUTE);
        phongshader->link();
    }

}

// Metode per inicialitzar els shaders de l'aplicacio
void GLWidget::initShadersGPU()
{
    // Carrega dels shaders i posa a punt per utilitzar els programes carregats a la GPU
    InitShader( "../Carreres/vshader11_flat.glsl", "../Carreres/fshader11_flat.glsl", "flat" );
    InitShader( "../Carreres/vshader11_gouraud.glsl", "../Carreres/fshader11_gouraud.glsl", "gouraud");
    InitShader( "../Carreres/vshader11_phong.glsl", "../Carreres/fshader11_phong.glsl", "phong" );

}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const

{
    return QSize(400, 400);
}

// Metodes per a poder rotar l'escena

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

/* Esta función no se utiliza en la práctica 2*/
void GLWidget::adaptaObjecteTamanyWidget(Objecte *obj) {

    obj->calculCapsa3D();
    obj->aplicaTG(Translate(obj->capsa.centro) * modScalat * Translate(-obj->capsa.centro));
    obj->calculCapsa3D();
}

void GLWidget::newObjecte(Objecte *obj)
{

    this->esc->addObjecte(obj);
    this->esc->actualitzaCameraPanoramica(false);
    updateGL();
}

// Pasamos el fichero './DataSet/obstacle.obj' realizado con blender que cargará 'n' obstáculos en la escena.

void GLWidget::newObstacle(QString fileName, int nombre)
{
    // Metode que serveix per a donar d'alta un obstacle amb el punt original a xorig, yorig,zorig
    // d'una certa mida
    // ha d'estar a les ys del pla de la terra

    GLfloat xorig = 0.0;
    GLfloat yorig = 0.0;
    GLfloat zorig = 0.0;

    for(int i = 0; i < nombre; i++){

        obstacle *obs = new obstacle(fileName, nombre, xorig, yorig, zorig);
        this->esc->list_obstacle.append(obs);         // <-- Guardamos todos los obstáculos de la escena.
        this->esc->c_materials->addMaterial(obs->getMaterial());
        this->esc->c_lights->addLlum(obs->getLlum());
        newObjecte(obs);
    }


}


void GLWidget::newTerra(float amplaria, float profunditat, float y)
{
    // Metode que crea un objecte terra poligon amb el punt original a xorig, yorig, zorig
    // (quadrat d'una certa mida amb origen a xorig, yorig, zorig

    GLfloat xorig = 0.0;
    GLfloat yorig = 0.0;
    GLfloat zorig = 0.0;

    Terra *terra =  new Terra(amplaria, profunditat, (y-0.5), xorig, yorig, zorig);  
    this->esc->c_materials->addMaterial(terra->getMaterial());
    this->esc->c_lights->addLlum(terra->getLlum());
    newObjecte(terra);

 }


void GLWidget::newCotxe(QString fichero, float xorig, float zorig, float mida, float xdirec, float ydirec, float zdirec)
{
    // parametres que posen l'objecte cotxe al punt original xorig, yorig, zorig i d'una certa mida
    // Cal modificar-lo per a que es posicioni a la Y correcte

    Cotxe *obj;
    float yorig = 0;

    if(esc->list_cotxe.size() < 2){

        obj = new Cotxe(fichero, mida, xorig, yorig, zorig, 0., 0., 0.,xdirec, ydirec, zdirec);
        this->esc->list_cotxe.append(obj);            //<-- Guardamos todos los coches de la escena.
        this->esc->c_materials->addMaterial(obj->getMaterialCarroceria());
        this->esc->c_lights->addLlum(obj->getLlumCarroceria());
        this->esc->c_materials->addMaterial(obj->getMaterialRoda());
        this->esc->c_lights->addLlum(obj->getLlumRoda());
        newObjecte(obj);

        esc->iniLookAtCotxe();
        esc->camaraThirdPerson->toGPU(phongshader);
        updateGL();
    }else
        cout<<"Numero maximo de coches añadidos!!!"<<endl;


}


void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    std::cout<<"Estic inicialitzant el shaders"<<std::ends;
    initShadersGPU();

    this->cameraActual->ini(this->size().width(), this->size().height(), esc->capsaMinima);
    // A canviar per posar una imatge de fons: Estrelles...
    qglClearColor(qtPurple.dark());
}

void GLWidget::resetView()
{
    xRot = 0;
    yRot = 0;
    zRot = 0;


    esc->reset();
    updateGL();
    esc->resetCameraPanoramica();
    esc->actualitzaCameraPanoramica(false);

    for(int i = 0; i < esc->list_cameras.size(); i++)
        esc->resetCameraThirdPerson(i);

    third = false;

    updateGL();
}

void GLWidget::paintGL()
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    qNormalizeAngle(xRot);
    qNormalizeAngle(yRot);
    qNormalizeAngle(zRot);

    mat4 transform = (  RotateX( xRot / 16.0 ) *
                        RotateY( yRot / 16.0 ) *
                        RotateZ( zRot / 16.0 ) );

    /* Al tener la lista aplicamos directamente*/
    esc->aplicaTGCentrat(transform);


    /* Según la vista actual mostraremos una camara u otra*/
    if(!third){
        flatshader->bind();
        this->cameraActual->toGPU(flatshader);
        gouraudshader->bind();
        this->cameraActual->toGPU(gouraudshader);
        phongshader->bind();
        this->cameraActual->toGPU(phongshader);
    }else{
        flatshader->bind();
        this->esc->list_cameras.at(index-1)->toGPU(flatshader);
        gouraudshader->bind();
        this->esc->list_cameras.at(index-1)->toGPU(gouraudshader);
        phongshader->bind();
        this->esc->list_cameras.at(index-1)->toGPU(phongshader);
    }

    esc->draw(flatshader, gouraudshader, phongshader);

}


void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-150, +150, -150, +150, 0.0, 150.0);
#else
    glOrtho(-150, +150, -150, +150, 0.0, 150.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}


/* Rotamos la camara en la vista panorámica*/
void GLWidget::setXRotation(int angle)
{
    if(!third){
        /* La camara al rotar en "x" tenemos que cambiar el angulo en "y" y restando para que
         * vaya en la dirección del ratón*/
        this->cameraActual->vs.angy =  (int)(this->cameraActual->vs.angy - angle) % 360;
        this->esc->calculosCameraPanoramica();
        this->esc->actualitzaCameraPanoramica(false);

        updateGL();
    }
}

/* Rotamos la camara en la vista panorámica*/
void GLWidget::setYRotation(int angle)
{
    if(!third) {
        /* La camara al rotar en y tenemos que cambiar el angulo en x y restando para que
         * vaya en la dirección del ratón*/
        this->cameraActual->vs.angx =  (int)(this->cameraActual->vs.angx - angle) % 360;
        this->esc->calculosCameraPanoramica();
        this->esc->actualitzaCameraPanoramica(false);
        updateGL();
    }
}


void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        updateGL();
    }
}


/* Hacemos zoom cuando tenemos vista panorámica*/
void GLWidget::Zoom(float zoom)
{
    if(!third){
        this->cameraActual->AmpliaWindow(zoom);
        this->esc->actualitzaCameraPanoramica(false);
        updateGL();
    }
}

/* Movemos la camara en varios ejes*/
void GLWidget::Pan(int dx, int dy)
{
    if(/*x != dx && */dx < 0)
        this->cameraActual->wd.pmin.x += 0.05;
    else
        this->cameraActual->wd.pmin.x -= 0.05;

    if(/*y != dy && */dy > 0)
        this->cameraActual->wd.pmin.y += 0.05;
    else
        this->cameraActual->wd.pmin.y -= 0.05;

    this->x = dx;
    this->y = dy;

    this->esc->actualitzaCameraPanoramica(false);

    updateGL();
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event){

    int dx = event->x() - this->lastPos.x();
    int dy = event->y() - this->lastPos.y();

    if (event->buttons() & Qt::LeftButton){
        if(this->lastPos.y()!= event->y() && this->lastPos.x()!= event->x()) {
        // Moure convenientment la càmera en angle X i/o en angle Y
            this->setXRotation(dx);
            this->setYRotation(dy);

        } else if(this->lastPos.y()!= event->y()) {
            // Moure convenientment la càmera en angle X i/o en angle Y
            this->setYRotation(dy);

        } else if (this->lastPos.x()!= event->x()) {
            // Moure convenientment la càmera en angle X i/o en angle Y
            this->setXRotation(dx);
        }
    } else if (event->buttons() & Qt::RightButton) {
        // Panning: moure el centre de la window
        Pan(dx, dy);

    } else if (event->buttons() & Qt::MidButton) {
    // Zoom: canviar la mida de la window un tant per cent
        if(this->lastPos.y()> event->y())
            this->Zoom(-0.05);
        else
            this->Zoom(0.05);
    }

    this->lastPos = event->pos();
}

/* Al girar la rueda del ratón hacemos zoom*/
void GLWidget::wheelEvent(QWheelEvent *event){

    if(event->delta() < 0)
        this->Zoom(-0.05);
    else
        this->Zoom(0.05);
}



// Extensiones realizadas:

//   o) El coche se desplaza adelante, atrás, izq, der. diagonal etc.
//   o) El coche aumenta de velocidad tanto para adelante como hacia atrás dejando pulsado la tecla.
//   o) Es posible incluir otro coche y manejarlo con las teclas w,a,s,d.
//   o) Introducir 'n' obstaculos a traves del fichero './dataSet/obstacles.obj'

// Eventos que realiza:

//  o) Flecha Up movemos hacia adelante el primer coche de la escena.
//  o) Flecha Down movemos hacia atrás el primer coche de la escena.
//  o) Flecha Left rotamos hacia la izquierda el primer coche de la escena.
//  o) Flecha Rigth rotamos hacia la derecha el primer coche de la escena.

//  o) Flecha W movemos hacia adelante el resto de coches de la escena, excepción del primero.
//  o) Flecha A movemos hacia atrás el el resto de coches de la escena, excepción del primero.
//  o) Flecha S rotamos hacia la izquierda el resto de coches de la escena, excepción del primero.
//  o) Flecha E rotamos hacia la derecha el resto de coches de la escena, excepción del primero.


void GLWidget::keyPressEvent(QKeyEvent *event)
{
    int girar;
    bool obstacle;

    switch ( event->key() ) {

        case Qt::Key_Up:
            // Recorremos todos los coches para avanzar.
            if(!esc->list_cotxe.empty()){

                obstacle = esc->isObstacle(0);
                //esc->list_cotxe.at(0)->dir = FORWARD;

                if(!obstacle){
                    esc->list_cotxe.at(0)->speed += 0.003 * esc->list_cotxe.at(0)->scale;
                    esc->list_cotxe.at(0)->direction[0] = -1;
                    esc->list_cotxe.at(0)->forward();

                    if(third && index != 0)
                        this->esc->actualitzaCameraThirdPerson(index-1);
                    else
                        this->esc->actualitzaCameraPanoramica(false);

                    esc->list_cotxe.at(0)->dir = FORWARD;

                }else if(obstacle  && esc->list_cotxe.at(0)->dir == BACKWARD){
                        esc->list_cotxe.at(0)->speed = 0.025 * esc->list_cotxe.at(0)->scale;
                        esc->list_cotxe.at(0)->direction[0] = -1;
                        esc->list_cotxe.at(0)->forward();
                }

                updateGL();
            }
            break;
        case Qt::Key_Down:
            // Recorremos todos los coches para avanzar.
            if(!esc->list_cotxe.empty()){

                obstacle = esc->isObstacle(0);

                if(!obstacle){
                    esc->list_cotxe.at(0)->speed += 0.003 * esc->list_cotxe.at(0)->scale;
                    esc->list_cotxe.at(0)->direction[0] = 1;
                    esc->list_cotxe.at(0)->backward();

                    if(third && index != 0)
                        this->esc->actualitzaCameraThirdPerson(index-1);
                    else
                        this->esc->actualitzaCameraPanoramica(false);

                    esc->list_cotxe.at(0)->dir = BACKWARD;

                }else if(obstacle  && esc->list_cotxe.at(0)->dir == FORWARD){
                    esc->list_cotxe.at(0)->speed = 0.025 * esc->list_cotxe.at(0)->scale;
                    esc->list_cotxe.at(0)->direction[0] = 1;
                    esc->list_cotxe.at(0)->backward();
                }

                // Refrescamos la escena.
                updateGL();
            }
            break;
        case Qt::Key_Left:
            // Recorremos todos los coches para avanzar.
            if(!esc->list_cotxe.empty()){

                if(!esc->isObstacle(0)){
                    esc->list_cotxe.at(0)->giro += 9;

                    if(third && index == 1){
                        this->esc->list_cameras.at(index-1)->vs.angy -= 9; //<-- Hay que arreglar la rot del coche antes
                        this->esc->actualitzaCameraThirdPerson(index-1);
                    }else if(!third && index == 0){
                        this->esc->list_cameras.at(0)->vs.angy -= 9;
                        this->esc->actualitzaCameraPanoramica(false);
                    }

                    girar = esc->list_cotxe.at(0)->giro;

                    if(abs(girar) % 180 == 0)
                        esc->list_cotxe.at(0)->direction[2] = 0;
                    else
                        esc->list_cotxe.at(0)->direction[2] = 1;

                    esc->list_cotxe.at(0)->turnleft();

                }

                // Refrescamos la escena.
                updateGL();
            }
            break;

        case Qt::Key_Right:
            // Recorremos todos los coches para avanzar.
            if(!esc->list_cotxe.empty()){

                if(!esc->isObstacle(0)){

                    if(esc->list_cotxe.at(0)->giro == 0)
                        esc->list_cotxe.at(0)->giro = 360;

                    esc->list_cotxe.at(0)->giro -= 9;
                    girar = esc->list_cotxe.at(0)->giro;

                    if(third && index == 1){
                        this->esc->list_cameras.at(index-1)->vs.angy += 9; //<-- Hay que arreglar la rot del coche antes
                        this->esc->actualitzaCameraThirdPerson(index-1);
                    }else if(!third && index == 0){
                        this->esc->list_cameras.at(0)->vs.angy += 9;
                        this->esc->actualitzaCameraPanoramica(false);
                    }


                    if(abs(girar) % 180 == 0)
                        esc->list_cotxe.at(0)->direction[2] = 0;
                    else
                        esc->list_cotxe.at(0)->direction[2] = -1;

                    esc->list_cotxe.at(0)->turnright();

                }

                // Refrescamos la escena.
                updateGL();
            }
            break;

        case Qt::Key_W:
            // Recorremos todos los coches para avanzar.

            if(this->esc->list_cotxe.size() > 1){

                for(int i = 1; i < this->esc->list_cotxe.size(); i++){

                    obstacle = esc->isObstacle(i);

                    if(!obstacle){
                        esc->list_cotxe.at(i)->speed += 0.003 * esc->list_cotxe.at(i)->scale;
                        esc->list_cotxe.at(i)->direction[0] = -1;
                        esc->list_cotxe.at(i)->forward();



                        if(third  && index != 0)
                            this->esc->actualitzaCameraThirdPerson(index-1);
                        else
                            this->esc->actualitzaCameraPanoramica(false);

                        esc->list_cotxe.at(i)->dir = FORWARD;

                    }else if(obstacle  && esc->list_cotxe.at(i)->dir == BACKWARD){
                        esc->list_cotxe.at(i)->speed = 0.025 * esc->list_cotxe.at(i)->scale;
                        esc->list_cotxe.at(i)->direction[0] = -1;
                        esc->list_cotxe.at(i)->forward();
                    }
                }

            }
            // Refrescamos la escena.
            updateGL();
            break;

        case Qt::Key_S:
            // Recorremos todos los coches para avanzar.

            obstacle = esc->isObstacle(0);

            if(this->esc->list_cotxe.size() > 1){

                for(int i = 1; i < this->esc->list_cotxe.size(); i++){

                    obstacle = esc->isObstacle(i);

                    if(!obstacle){
                        esc->list_cotxe.at(i)->speed += 0.003 * esc->list_cotxe.at(i)->scale;
                        esc->list_cotxe.at(i)->direction[0] = 1;
                        esc->list_cotxe.at(i)->backward();


                    if(third  && index != 0)
                        this->esc->actualitzaCameraThirdPerson(index-1);
                    else
                        this->esc->actualitzaCameraPanoramica(false);

                    esc->list_cotxe.at(i)->dir = BACKWARD;

                    }else if(obstacle  && esc->list_cotxe.at(i)->dir == FORWARD){
                        esc->list_cotxe.at(i)->speed = 0.025 * esc->list_cotxe.at(i)->scale;
                        esc->list_cotxe.at(i)->direction[0] = 1;
                        esc->list_cotxe.at(i)->backward();
                    }

                }
            }

            // Refrescamos la escena.
            updateGL();
            break;

        case Qt::Key_A:
            // Recorremos todos los coches para avanzar.
            if(this->esc->list_cotxe.size() > 1){
                for(int i = 1; i < this->esc->list_cotxe.size(); i++){
                    esc->list_cotxe.at(i)->giro += 9;
                    girar = esc->list_cotxe.at(i)->giro;

                    if(abs(girar) % 180 == 0)
                        esc->list_cotxe.at(i)->direction[2] = 0;
                    else
                        esc->list_cotxe.at(i)->direction[2] = 1;

                    esc->list_cotxe.at(i)->turnleft();
                }


                 // Parece que va mal pero no es asi es culpa de la rotacion del coche al centro de la escena
                if(third && index == 2){
                    this->esc->list_cameras.at(index-1)->vs.angy -= 9; //<-- Hay que arreglar la rot del coche antes
                    this->esc->actualitzaCameraThirdPerson(index-1);
                }else if(!third && index == 0){
                    this->esc->list_cameras.at(1)->vs.angy -= 9;
                    this->esc->actualitzaCameraPanoramica(false);
                }

            }
            // Refrescamos la escena.
            updateGL();
            break;

        case Qt::Key_D:
            // Recorremos todos los coches para avanzar.
            if(this->esc->list_cotxe.size() > 1){
                for(int i = 1; i < this->esc->list_cotxe.size(); i++){
                    if(esc->list_cotxe.at(i)->giro == 0)
                        esc->list_cotxe.at(i)->giro = 360;

                    esc->list_cotxe.at(i)->giro -= 9;
                    girar = esc->list_cotxe.at(i)->giro;

                    if(abs(girar) % 180 == 0)
                        esc->list_cotxe.at(i)->direction[2] = 0;
                    else
                        esc->list_cotxe.at(i)->direction[2] = -1;

                    esc->list_cotxe.at(i)->turnright();
                }




                 // Parece que va mal pero no es asi es culpa de la rotacion del coche al centro de la escena
                if(third && index == 2){
                    this->esc->list_cameras.at(index-1)->vs.angy += 9; //<-- Hay que arreglar la rot del coche antes
                    this->esc->actualitzaCameraThirdPerson(index-1);
                }else if(!third && index == 0){
                    this->esc->list_cameras.at(1)->vs.angy += 9;
                    this->esc->actualitzaCameraPanoramica(false);
                }



            }
            // Refrescamos la escena.
            updateGL();
            break;

        case Qt::Key_Escape:
            if(this->esc->list_cotxe.size() > 0){

                index++;

                cout<<"Index: "<<index%(esc->list_cameras.size()+1)<<" isThird: "<<third<<endl;
                this->index = index%(esc->list_cameras.size()+1);
                third = true;

                if(third && index != 0){
                    this->esc->actualitzaCameraThirdPerson(index-1);
                    this->esc->list_cameras.at(index-1)->toGPU(flatshader);
                    this->esc->list_cameras.at(index-1)->toGPU(gouraudshader);
                    this->esc->list_cameras.at(index-1)->toGPU(phongshader);

                }else{
                    this->esc->resetCameraPanoramica();
                    third = false;
                }
                updateGL();
                updateGL();
            }
            break;
        }

}


// Este evento permite que si dejamos pulsado el botón de avanzar o retroceder aumenta la velocidad del coche.

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{

    if(event->isAutoRepeat())
        event->ignore();
    else{
        switch ( event->key() )
        {
            case Qt::Key_Up:
                if(!esc->list_cotxe.empty()){
                    esc->list_cotxe.at(0)->speed = 0.02 * esc->list_cotxe.at(0)->scale;
                }


            break;
            case Qt::Key_Down:
                if(!esc->list_cotxe.empty()){
                    esc->list_cotxe.at(0)->speed = 0.02 * esc->list_cotxe.at(0)->scale;
                }


            break;
            case Qt::Key_W:
                if(this->esc->list_cotxe.size() > 1){
                    for(int i = 1; i < this->esc->list_cotxe.size(); i++){
                        esc->list_cotxe.at(i)->speed = 0.02 * esc->list_cotxe.at(i)->scale;
                    }
                }
            break;
            case Qt::Key_S:
                if(this->esc->list_cotxe.size() > 1){
                    for(int i = 1; i < this->esc->list_cotxe.size(); i++){
                        esc->list_cotxe.at(i)->speed = 0.02 * esc->list_cotxe.at(i)->scale;
                    }
                }
            break;
        }
    }
}
