#include "cotxe.h"

Cotxe::Cotxe(QString n) : Objecte(NumVerticesF)
{
    nom = n;
    Index = 0;
    tam = 1;
    scale = tam;

    xorig = 0;
    yorig = 0;
    zorig = 0;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    this->direction[0] = -1;
    this->direction[1] = 0;
    this->direction[2] = 0;
    this->direction[3] = 0;

    //this->dir = BACKWARD;
    Ks = vec3(0.727811, 0.626959, 0.626959); Kd = vec3(0.61424, 0.04136, 0.04136); Ka = vec3(0.1745, 0.01175, 0.01175);
    material = new Material("coche", Kd, Ks, Ka, 0.6);

    readObj(n);
    make();

}

Cotxe::Cotxe(QString n, GLfloat tamanio, GLfloat x0, GLfloat y0, GLfloat z0,
             double girx, double giry, double girz,
             float xdir, float ydir, float zdir):Objecte(NumVerticesF, n, tamanio, x0, y0, z0, girx, giry, girz)
{
    // El seguent codi escala el cotxe entre 0 i 1 i el situa el seu centre  0,0,0. aixo fa que es vegi en la primera visualització
    //

    if(tamanio == 0)
        tam = 1;
    else
        tam = tamanio;

    scale = tam;

    xorig = x0;
    yorig = y0;
    zorig = z0;

    xRot = xdir;
    yRot = ydir;
    zRot = zdir;

    this->direction[0] = xdir;
    this->direction[1] = ydir;
    this->direction[2] = zdir;
    this->direction[3] = 0;

    this->speed = 0.02;

    Ks = vec3(0.727811, 0.626959, 0.626959); Kd = vec3(0.61424, 0.04136, 0.04136); Ka = vec3(0.1745, 0.01175, 0.01175);
    material = new Material("coche", Kd, Ks, Ka, 0.6);

    readObj(n);
    make();

}


void Cotxe::readObj(QString filename)
{

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        Objecte *partes;

        int vindexAct = 0;
        int vindexUlt = 0;

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4)
                {
                    fprintf (stderr, "Too few coordinates");//: '%s'", str_orig);
                    exit (-1);
                }
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5)
                {
                  QString sw(ReadFile::words[4]);
                  double w = sw.toDouble();
                  x/=w;
                  y/=w;
                  z/=w;
                }
                // S'afegeix el vertex a l'objecte
                partes->vertexs.push_back(point4(x, y, z, 1));
                vindexAct++;

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                construeix_cara (&ReadFile::words[1], nwords-1, partes, vindexUlt);
            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            else if (!strcmp (first_word, "o")) {
                //cada nou objecte s'actualitza aquest Ã­ndex
                vindexUlt = vindexAct;

                /* Objeto partes*/
                partes = NULL;

                /* Comparamos el texto y creamos la parte del coche*/
                if(!strcmp(ReadFile::words[1], "Roda_Esquerra_Posterior_Untitled")){
                    TraseraI = new Roda();
                    partes = TraseraI;
                    partes_coche.append(TraseraI);
                }
                else if(!strcmp(ReadFile::words[1], "Roda_Dreta_Posterior_04")){
                    TraseraD = new Roda();
                    partes = TraseraD;
                    partes_coche.append(TraseraD);
                }
                else if(!strcmp(ReadFile::words[1], "Roda_Esquerra_Davantera_02")){
                    DelanteraI = new Roda();
                    partes = DelanteraI;
                    partes_coche.append(DelanteraI);
                }
                else if(!strcmp(ReadFile::words[1], "Roda_Dreta_Davantera_03")){
                    DelanteraD = new Roda();
                    partes = DelanteraD;
                    partes_coche.append(DelanteraD);
                }
                else if(!strcmp(ReadFile::words[1], "Carrosseria_00")){
                    chasis = new Carrosseria();
                    partes = chasis;
                    partes_coche.append(chasis);
                }
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }

            //free(words);
        }
    }

}


Capsa3D Cotxe::calculCapsa3D(){


    vec3    pmin, pmax;
    vec3    capsa_min, capsa_max;

    for(int i = 0; i < partes_coche.size(); i++)
        partes_coche.at(i)->calculCapsa3D();

    pmin = partes_coche.at(0)->capsa.pmin;
    pmax = partes_coche.at(0)->capsa.pmax;

    for(int j = 0; j < partes_coche.size(); j++){
        capsa_min = partes_coche.at(j)->capsa.pmin;
        capsa_max = partes_coche.at(j)->capsa.pmax;

        pmin.x = min(pmin.x, capsa_min.x);
        pmin.y = min(pmin.y, capsa_min.y);
        pmin.z = min(pmin.z, capsa_min.z);

        pmax.x = max(pmax.x , capsa_max.x);
        pmax.y = max(pmax.y , capsa_max.y);
        pmax.z = max(pmax.z , capsa_max.z);

        // DEBUG
        //cout<<"x: "<<pmin.x<<", y: "<<pmin.y<<", z: "<<pmin.z<<", CapsaMin: "<<pmin<<endl;
        //cout<<"x: "<<pmax.x<<", y: "<<pmax.y<<", z: "<<pmax.z<<", CapsaMax: "<<pmax<<endl;

    }

    capsa.pmin = pmin;
    capsa.pmax = pmax;
    capsa.a = pmax.x - pmin.x;
    capsa.h = pmax.y - pmin.y;
    capsa.p = pmax.z - pmin.z;

    // Calculamos el centro de la caja
    capsa.centro.x = (capsa.pmin.x + capsa.a / 2);
    capsa.centro.y = (capsa.pmin.y + capsa.h / 2);
    capsa.centro.z = (capsa.pmin.z + capsa.p / 2);

    // Calculamos el lado más grande para poder escalar
    capsa.unitario = max(capsa.a, capsa.h);
    capsa.unitario = max(capsa.unitario, capsa.p);

    // DEBUG
    /*cout<<"\nCAPSA3D COTXE\nWidth: "<<capsa.a<<"\nHeigth: "<<capsa.h<<"\nDepth: "<<capsa.p
       <<"\nCentro: "<<capsa.centro<<"\nUnitario: "<<capsa.unitario
       <<"\nCapsaMin: "<<capsa.pmin<<"\nCapsaMax: "<<capsa.pmax<<endl;*/
    return capsa;
}


/* Método make sobrescrito*/
void Cotxe::make(){

    int x0 = -1;
    int y0 = 0;
    int z0 = 0;

    // Calculamos el ángulo del vector director.

    float angulo = acos((x0*xRot + y0*yRot + z0*zRot) / (sqrt(pow(x0, 2)+pow(y0, 2)+pow(z0, 2))
            *sqrt(pow(xRot, 2)+pow(yRot, 2)+pow(zRot, 2)))) * 180.0 / M_PI;

    mat4 rotate = RotateY(angulo);

    // NOTA!!! Si esta aplicado el adaptaWidgetTamanyPantalla dejarlo tal cual sino invertir
    // el signo del RotateY(); de dentro de el if/else.
    // Rotaremos el coche según el ángulo dado por el vector director.

    if(zRot < 0){
        this->giro = 360 - angulo;
        rotate = RotateY(angulo);
    }else{
        this->giro = angulo;
        rotate = RotateY(-angulo);
    }

    // Hacemos un make() de cada parte del coche.
    cout<<"Partes Coche: "<<partes_coche.size()<<endl;
    for(int i = 0; i < partes_coche.size(); i++){
        partes_coche.at(i)->make();
        partes_coche.at(i)->gouraud();
    }

    // Calculamos la caja 3D del coche
    calculCapsa3D();

    // Escalamos el coche unitariamente.
    GLfloat escala = 1.0 / capsa.unitario;

    // Operacion de der-->izq:
    //  o) Trasladar al origen.
    //  o) Escalado unitario.
    //  o) Escalado del User.
    //  o) Devolver el objeto.

    mat4 m = Translate(xorig, yorig ,zorig)
            *rotate
            *Scale(tam, tam, tam)
            *Scale(escala, escala, escala)
            *Translate(-capsa.centro);

    aplicaTG(m);
    // Recalculamos la nueva caja 3D
    calculCapsa3D();

    /* Ponemos el coche encima del terra. Como tenemo una escala tenemos que dividir
       sino nos pasamos del planto*/
    aplicaTG(Translate(0, abs(capsa.pmax.y)/tam, 0));
    calculCapsa3D();

}

/* Método toGPU sobrescrito*/
void Cotxe::toGPU(QGLShaderProgram *p){
    for(int i = 0; i < partes_coche.size(); i++){
        partes_coche.at(i)->toGPU(p);
    }
    // Enviamos el material a la GPU


}

/* Método draw sobrescrito*/
void Cotxe::draw(){
    for(int i = 0; i < partes_coche.size(); i++)
        partes_coche.at(i)->draw();
}

/* Método aplicaTG sobrescrito*/
void Cotxe::aplicaTG(mat4 m){
    for(int i = 0; i < partes_coche.size(); i++)
        partes_coche.at(i)->aplicaTG(m);
}

/* Método aplicaTGPoints sobrescrito*/
void Cotxe::aplicaTGPoints(mat4 m){
    for(int i = 0; i < partes_coche.size(); i++)
        partes_coche.at(i)->aplicaTGPoints(m);
}


void Cotxe::aplicaTGCentrat(mat4 m){

    calculCapsa3D();

    mat4 op = Translate(capsa.centro) * m * Translate(-capsa.centro);
    aplicaTG(op);
}


void Cotxe::forward(){


    // Avanzamos el coche por la escena

    // Si el coche mira hacia la izq. sobre el eje de las 'x' y no estamos sobre 'z'(vease girado)
    // y el ángulo es de 0º avanzamos el coche sobre el eje de las -x y giramos las ruedas.

    if(this->direction[0] == -1 && this->direction[2] == 0 && giro == 0){
        aplicaTG(Translate(-speed,0,0));
        RotarRuedas(RotateZ(6));
    }

    // Si el coche mira hacia la izq. sobre el eje de las 'x' y estamos sobre 'z'(vease girado)
    // y el ángulo esta entre 1º y 89º avanzamos el coche en diagonal izquierda sobre el eje de las '-x' y 'z'.

    if(this->direction[0] == -1 && this->direction[2] != 0 && (giro > 0 && giro < 90)){
        aplicaTG(Translate(-speed,0,-speed));
    }

    // Si el coche mira hacia arriba sobre el eje de las 'x' y estamos sobre 'z'(vease girado)
    // y el ángulo es 90 avanzamos hacia adelante el coche sobre el eje de las '-x' y 'z'.

    if(this->direction[0] == -1 && this->direction[2] != 0 && giro == 90){
        aplicaTG(Translate(0,0,-speed));
        RotarRuedas(RotateX(6));
    }

    // Si el coche mira hacia la izq. sobre el eje de las 'x' y estamos sobre 'z'(vease girado)
    // y el ángulo esta entre 91º y 179º avanzamos el coche en diagonal derecha sobre el eje de las 'x' y 'z'.

    if(this->direction[0] == -1 && this->direction[2] != 0 && (giro > 90 && giro < 180)){
        aplicaTG(Translate(speed,0,-speed));
    }

    // Si el coche mira hacia la derecha sobre el eje de las 'x' y no estamos sobre 'z'(vease girado)
    // y el ángulo es 180 avanzamos hacia el coche hacia la derecha sobre el eje de las 'x'.

    if(this->direction[0] == -1 && this->direction[2] == 0 && giro == 180){
        aplicaTG(Translate(speed,0,0));
        RotarRuedas(RotateZ(-6));
    }

    // Si el coche mira hacia la derecha sobre el eje de las 'x' y estamos sobre '-z'(vease girado)
    // y el ángulo esta entre 181º y 269º avanzamos el coche en diagonal derecha sobre el eje de las 'x' y '-z'.

    if(this->direction[0] == -1 && this->direction[2] != 0 && (giro > 180 && giro < 270)){
        aplicaTG(Translate(speed,0,speed));
    }

    // Si el coche mira hacia abajo sobre el eje de las 'x' y estamos sobre '-z'(vease girado)
    // y el ángulo es 270 avanzamos hacia abajo el coche sobre el eje de las 'x' y '-z'.

    if(this->direction[0] == -1 && this->direction[2] != 0 && giro == 270){
        aplicaTG(Translate(0,0,speed));
    }

    // Si el coche mira hacia la izquierda sobre el eje de las 'x' y estamos sobre '-z'(vease girado)
    // y el ángulo esta entre 271º y 359º avanzamos el coche en diagonal derecha sobre el eje de las 'x' y '-z'.

    if(this->direction[0] == -1 && this->direction[2] != 0 && (giro > 270 && giro < 360)){
        aplicaTG(Translate(-speed,0,speed));
    }


    calculCapsa3D();

}

// El método de ir marcha atrás es el mismo que el de 'forward()' pero en sentido contrario.

void Cotxe::backward(){

    // Avanzamos el coche por la escena

    if(this->direction[0] == 1 && this->direction[2] == 0 && giro == 0){
        aplicaTG(Translate(speed,0,0));
        RotarRuedas(RotateZ(6));
    }

    if(this->direction[0] == 1 && this->direction[2] != 0 && (giro > 0 && giro < 90)){
        aplicaTG(Translate(speed,0,speed));
    }

    if(this->direction[0] == 1 && this->direction[2] != 0 && giro == 90){
        aplicaTG(Translate(0,0,speed));
        RotarRuedas(RotateX(6));
    }

    if(this->direction[0] == 1 && this->direction[2] != 0 && (giro > 90 && giro < 180)){
        aplicaTG(Translate(-speed,0,speed));
    }

    if(this->direction[0] == 1 && this->direction[2] == 0 && giro == 180){
        aplicaTG(Translate(-speed,0,0));
        RotarRuedas(RotateZ(6));
    }

    if(this->direction[0] == 1 && this->direction[2] != 0 && (giro > 180 && giro < 270)){
        aplicaTG(Translate(-speed,0,-speed));
    }

    if(this->direction[0] == 1 && this->direction[2] != 0 && giro == 270){
        aplicaTG(Translate(0,0,-speed));
    }

    if(this->direction[0] == 1 && this->direction[2] != 0 && (giro > 270 && giro < 360)){
        aplicaTG(Translate(speed,0,-speed));
    }

    //cout<<"\nDirection: "<<direction<<endl;
    //cout<<"\nAngulo: "<<giro<<endl;
    calculCapsa3D();


}

// Método 'turnleft' rotamos las ruedas 'x' grados y rotamos el coche en dirección izq.

void Cotxe::turnleft(){

    if(this->angulo_rueda > -24){
        // Rotamos las ruedas delanteras
        DelanteraD->aplicaTGCentrat(RotateY(-6));
        DelanteraI->aplicaTGCentrat(RotateY(-6));
        angulo_rueda += -6;
    }

    if(this->giro == 360)
        this->giro = 0;

    aplicaTGCentrat(RotateY(-9));
    calculCapsa3D();
}

// Método 'turnright' rotamos las ruedas 'x' grados y rotamos el coche en dirección der.

void Cotxe::turnright(){

    if(DelanteraD != NULL && DelanteraI != NULL){
        if(this->angulo_rueda < 24){
            // Rotamos las ruedas delanteras
            DelanteraD->aplicaTGCentrat(RotateY(6));
            DelanteraI->aplicaTGCentrat(RotateY(6));
            angulo_rueda += 6;
        }


        if(this->giro == 360)
            this->giro = 0;

        aplicaTGCentrat(RotateY(9));
        calculCapsa3D();
    }
}

void Cotxe::RotarRuedas(mat4 m){

    for(int i = 0; i < partes_coche.size(); i++)
        if(dynamic_cast<Roda*>(partes_coche.at(i)))
            partes_coche.at(i)->aplicaTGCentrat(m);
}

void Cotxe::getCarroceriatoGPU(QGLShaderProgram *p){
    for(int i = 0; i < partes_coche.size(); i++){
        if(dynamic_cast<Carrosseria*>(partes_coche.at(i))){
            dynamic_cast<Carrosseria*>(partes_coche.at(i))->toGPU(p);
        }
    }
}

void Cotxe::getRodatoGPU(QGLShaderProgram *p){
    for(int i = 0; i < partes_coche.size(); i++){
        if(dynamic_cast<Roda*>(partes_coche.at(i))){
            dynamic_cast<Roda*>(partes_coche.at(i))->toGPU(p);
        }
    }
}

Material* Cotxe::getMaterialCarroceria(){

    for(int i = 0; i < partes_coche.size(); i++){
        if(dynamic_cast<Carrosseria*>(partes_coche.at(i))){
            return dynamic_cast<Carrosseria*>(partes_coche.at(i))->getMaterial();
        }
    }
}

Material* Cotxe::getMaterialRoda(){

    for(int i = 0; i < partes_coche.size(); i++){
        if(dynamic_cast<Roda*>(partes_coche.at(i))){
            return dynamic_cast<Roda*>(partes_coche.at(i))->getMaterial();
        }
    }
}

Llum* Cotxe::getLlumCarroceria(){

    for(int i = 0; i < partes_coche.size(); i++){
        if(dynamic_cast<Carrosseria*>(partes_coche.at(i))){
            return dynamic_cast<Carrosseria*>(partes_coche.at(i))->getLlum();
        }
    }
}

Llum* Cotxe::getLlumRoda(){

    for(int i = 0; i < partes_coche.size(); i++){
        if(dynamic_cast<Roda*>(partes_coche.at(i))){
            return dynamic_cast<Roda*>(partes_coche.at(i))->getLlum();
        }
    }
}

Material* Cotxe::getMaterial(){
    return material;
}

Llum* Cotxe::getLight(){
    return light;
}

void Cotxe::doGouraud(){
    for(int i = 0; i < partes_coche.size(); i++){
        partes_coche.at(i)->gouraud();
    }
}
