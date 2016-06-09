#include <escena.h>

using namespace std;

escena::escena()

{
    // Capsa minima contenidora provisional: S'ha de fer un recorregut dels objectes de l'escenes
    capsaMinima.pmin[0] = 0; capsaMinima.pmin[1] = 0; capsaMinima.pmin[2]=0;
    capsaMinima.a = 1; capsaMinima.h = 1; capsaMinima.p = 1;

    camaraPanoramica = new Camera();

    this->c_lights = new conjuntllums();
    this->c_materials = new conjuntmaterials();

    //Creamos una Luz Puntual al inicializar la escena
    /*light = new Llum();

    Is = vec3(0.35, 0, 0); Id = vec3(1,1,1); Ia = vec3(1,0.5,0.5);
    light->LlumPuntual(vec4(0,5,0), Id, Is, Ia);
    light->setCoeficients(.25,.25,.25);*/

    // Añado la luz al conjunto de luces
    //this->c_lights->addLlum(light);

}


escena::~escena()
{
    printf("DESTRUCTOR!");

}

void escena::addObjecte(Objecte *obj) {

    this->objScene.append(obj);
    cout<<"Num objectes: "<<this->objScene.size()<<endl;
    this->resetCameraPanoramica();
}


bool escena::isObstacle(int idCar){

    Capsa3D car = list_cotxe.at(idCar)->capsa;
    Capsa3D obstacle;

    for(int i = 0; i < list_obstacle.size(); i++){
        obstacle = list_obstacle.at(i)->capsa;

        if(obstacle.pmin.x < car.pmax.x && obstacle.pmax.x > car.pmin.x
                && obstacle.pmin.z < car.pmax.z && obstacle.pmax.z > car.pmin.z){
            cout<<"Has chocado con un obstaculo"<<endl;
            return true;
        }

    }

    cout<<"Vas bien"<<endl;
    return false;
}

/* Calculamos la caja mínima de la escena.*/
void escena::CapsaMinCont3DEscena(){

    vec3    pmin, pmax;

    /* Recorremos todos los objetos de la escena.*/
    if(this->objScene.length() > 0){

        pmin = this->objScene.at(0)->capsa.pmin;
        pmax = this->objScene.at(0)->capsa.pmax;

        for(int i = 1; i < this->objScene.length(); i++){

            pmin.x = min(pmin.x, this->objScene.at(i)->capsa.pmin.x);
            pmin.y = min(pmin.y, this->objScene.at(i)->capsa.pmin.y);
            pmin.z = min(pmin.z, this->objScene.at(i)->capsa.pmin.z);

            pmax.x = max(pmax.x , this->objScene.at(i)->capsa.pmax.x);
            pmax.y = max(pmax.y , this->objScene.at(i)->capsa.pmax.y);
            pmax.z = max(pmax.z , this->objScene.at(i)->capsa.pmax.z);

        }


    /* Obtenemos los puntos mínimos, máximos, altura, anchura y profundidad*/
    capsaMinima.pmin = pmin;
    capsaMinima.pmax = pmax;
    capsaMinima.a = pmax.x - pmin.x;
    capsaMinima.h = pmax.y - pmin.y;
    capsaMinima.p = pmax.z - pmin.z;

    // Dejamos calculados estos valores por si más adelante los tenemos que necesitar

    // Calculamos el centro de la caja
    capsaMinima.centro.x = (capsaMinima.pmin.x + capsaMinima.a / 2);
    capsaMinima.centro.y = (capsaMinima.pmin.y + capsaMinima.h / 2);
    capsaMinima.centro.z = (capsaMinima.pmin.z + capsaMinima.p / 2);

    // Calculamos el lado más grande para poder escalar
    capsaMinima.unitario = max(capsaMinima.a, capsaMinima.h);
    capsaMinima.unitario = max(capsaMinima.unitario, capsaMinima.p);

    }else{
        // DEBUG
        cout<<"No hay ningún objeto sobre la escena."<<endl;

    }

}


void escena::aplicaTG(mat4 m) {

    for(int i = 0; i < objScene.size(); i++){
        objScene.at(i)->aplicaTG(m);
    }
}

void escena::aplicaTGCentrat(mat4 m) {

    for(int i = 0; i < objScene.size(); i++){
        objScene.at(i)->aplicaTGCentrat(m);
    }
}

/*void escena::draw() {

    for(int i = 0; i < objScene.size(); i++){
        objScene.at(i)->draw();
    }
}*/

void escena::draw(QGLShaderProgram *program_flat,QGLShaderProgram *program_gouraud,
                  QGLShaderProgram *program_phong) {

    for(int i=0;i<objScene.size();i++){
        if(dynamic_cast<Terra*>(objScene.at(i))){
            program_flat->bind();
            //this->c_lights->list_llum.at(0)->toGPU(program_flat);
            //this->c_lights->setAmbientToGPU(program_flat);
            dynamic_cast<Terra*>(objScene.at(i))->getLlum()->toGPU(program_flat);
            dynamic_cast<Terra*>(objScene.at(i))->getMaterial()->toGPU(program_flat);
            dynamic_cast<Terra*>(objScene.at(i))->toGPU(program_flat);
            dynamic_cast<Terra*>(objScene.at(i))->draw();
        }
        if(dynamic_cast<obstacle*>(objScene.at(i))){
            program_gouraud->bind();
            //this->c_lights->list_llum.at(0)->toGPU(program_gouraud);
            //this->c_lights->setAmbientToGPU(program_gouraud);
            dynamic_cast<obstacle*>(objScene.at(i))->getLlum()->toGPU(program_flat);
            dynamic_cast<obstacle*>(objScene.at(i))->getMaterial()->toGPU(program_gouraud);
            dynamic_cast<obstacle*>(objScene.at(i))->toGPU(program_gouraud);
            dynamic_cast<obstacle*>(objScene.at(i))->draw();
        }
        if(dynamic_cast<Cotxe*>(objScene.at(i))){
            //program_phong->bind();
            int len = dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.size();
            for(int j = 0; j < len; j++){
                if(dynamic_cast<Roda*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))){
                    program_phong->bind();
                    //this->c_lights->list_llum.at(0)->toGPU(program_phong);
                    //this->c_lights->setAmbientToGPU(program_phong);
                    dynamic_cast<Roda*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->getLlum()->toGPU(program_phong);
                    dynamic_cast<Roda*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->getMaterial()->toGPU(program_phong);
                    dynamic_cast<Roda*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->toGPU(program_phong);
                    dynamic_cast<Roda*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->draw();

                }else{
                    program_phong->bind();
                    //this->c_lights->list_llum.at(0)->toGPU(program_phong);
                    //this->c_lights->setAmbientToGPU(program_phong);
                    dynamic_cast<Carrosseria*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->getLlum()->toGPU(program_phong);
                    dynamic_cast<Carrosseria*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->getMaterial()->toGPU(program_phong);
                    dynamic_cast<Carrosseria*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->toGPU(program_phong);
                    dynamic_cast<Carrosseria*>(dynamic_cast<Cotxe*>(objScene.at(i))->partes_coche.at(j))->draw();

                }

            }
        }
        objScene.at(i)->draw();
    }
}

void escena::reset() {

    for(int i = 0; i < objScene.size(); i++){
        objScene.at(i)->make();
    }
}


void escena::actualitzaCameraPanoramica(bool clip){

    if(clip)
        this->camaraPanoramica->CalculWindowAmbRetallat();

    this->CapsaMinCont3DEscena();
    this->camaraPanoramica->CalculaMatriuModelView();
    this->camaraPanoramica->CalculaMatriuProjection();
}


/* Método para reiniciar la camara*/
void escena::resetCameraPanoramica(){

    /* Calculamos la caja de la escena ya que se han podido añadir objetos*/
    this->CapsaMinCont3DEscena();

    /* Situamos los ángulos por defectos*/
    this->camaraPanoramica->vs.angx = -90;
    this->camaraPanoramica->vs.angy = 0;
    this->camaraPanoramica->vs.angz = 0;


    /* Volvemos a calcular las matrices y la window*/
    this->calculosCameraPanoramica();
    this->camaraPanoramica->piram.proj = PARALLELA;
    this->actualitzaCameraPanoramica(false);
    this->camaraPanoramica->CalculWindow(capsaMinima);

}

void escena::calculosCameraPanoramica(){

    vec4 panoramica = capsaMinima.pmax - capsaMinima.pmin;

    camaraPanoramica->piram.d = sqrt(pow(panoramica[0], 2) + pow(panoramica[1], 2) + pow(panoramica[2], 2));

    camaraPanoramica->piram.d = 2 * camaraPanoramica->piram.d;

    camaraPanoramica->piram.dant = camaraPanoramica->piram.d/2;
    camaraPanoramica->piram.dpost = camaraPanoramica->piram.dant*3;

    camaraPanoramica->vs.obs = camaraPanoramica->CalculObs(camaraPanoramica->vs.vrp,
                                                           camaraPanoramica->piram.d,
                                                           camaraPanoramica->vs.angx,
                                                           camaraPanoramica->vs.angy);

}



/* Inicialización para la camara en tercera persona.*/
void escena::iniLookAtCotxe(){

    /* Creamos la camara para la tercera persona y establecemos los angulos*/
    camaraThirdPerson = new Camera();

    vec4 Tpersona = this->list_cotxe[0]->capsa.pmax - this->list_cotxe[0]->capsa.pmin;
    Tpersona[3] = 1;

    camaraThirdPerson->vs.angx = -10;
    camaraThirdPerson->vs.angy = 90;
    camaraThirdPerson->vs.angz = 0;


    camaraThirdPerson->piram.d = sqrt(pow(Tpersona[0], 2) + pow(Tpersona[1], 2) + pow(Tpersona[2], 2));

    camaraThirdPerson->piram.d = 2 * camaraThirdPerson->piram.d;

    camaraThirdPerson->piram.dant = camaraThirdPerson->piram.d/2;
    camaraThirdPerson->piram.dpost = camaraThirdPerson->piram.dant*3;

    camaraThirdPerson->vs.obs = camaraThirdPerson->CalculObs(camaraThirdPerson->vs.vrp,
                                                             camaraThirdPerson->piram.d,
                                                             camaraThirdPerson->vs.angx,
                                                             camaraThirdPerson->vs.angy);

    camaraThirdPerson->CalculWindow(this->list_cotxe[0]->capsa);

    camaraThirdPerson->piram.proj = PERSPECTIVA;

    this->list_cameras.append(camaraThirdPerson);

}


/* Método para reiniciar la camara*/
void escena::resetCameraThirdPerson(int idCar){

    /* Situamos los ángulos por defectos*/
    this->list_cameras.at(idCar)->vs.angx = -10;
    this->list_cameras.at(idCar)->vs.angy = 90;
    this->list_cameras.at(idCar)->vs.angz = 0;

}

void escena::actualitzaCameraThirdPerson(int idCar){

    this->CapsaMinCont3DEscena();
    this->calculaThirdPersonObservador(idCar);
    this->list_cameras.at(idCar)->CalculaMatriuModelView();
    this->list_cameras.at(idCar)->CalculaMatriuProjection();

}

void escena::calculaThirdPersonObservador(int idCar){

    vec4 observador = this->list_cotxe.at(idCar)->capsa.centro;
    observador[3] = 1.0;

    list_cameras.at(idCar)->vs.vrp = observador;
    list_cameras.at(idCar)->vs.obs = list_cameras.at(idCar)->CalculObs(list_cameras.at(idCar)->vs.vrp,
                                                                       list_cameras.at(idCar)->piram.d,
                                                                       list_cameras.at(idCar)->vs.angx,
                                                                       list_cameras.at(idCar)->vs.angy);
}
