#ifndef ESCENA_H
#define ESCENA_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>

#include <objecte.h>
#include <cotxe.h>
#include <terra.h>
#include <obstacle.h>
#include <camera.h>
#include <conjuntllums.h>
#include <conjuntmaterials.h>

using namespace std;

class escena
{
public:
    escena();
    ~escena();

    void addObjecte(Objecte *cotxe);
    void aplicaTG(mat4 m);

    void aplicaTGCentrat(mat4 m);
    void reset();

    void draw(QGLShaderProgram *program_flat,QGLShaderProgram *program_gouraud,
              QGLShaderProgram *program_phong);
    void CapsaMinCont3DEscena();

    bool isObstacle(int idCar);

    // Métodos de Camera
    void actualitzaCameraPanoramica(bool clip);
    void resetCameraPanoramica();
    void calculosCameraPanoramica();

    void actualitzaCameraThirdPerson(int idCar);
    void iniLookAtCotxe();
    void resetCameraThirdPerson(int idCar);
    void calculaThirdPersonObservador(int idCar);


    // Camara de la escena
    Camera *camaraPanoramica;
    Camera *camaraThirdPerson;

    QList<Camera*> list_cameras;

    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;

    // Objectes de l'escena: a modificar. Ara nomes té un objecte. Cal afegir un conjunt d'objectes
    Cotxe *cotxe;
    Terra *terra;

    QList<Objecte*> objScene;
    QList<Cotxe*> list_cotxe;
    QList<obstacle*> list_obstacle;

    // Luz
    conjuntllums* c_lights;

    // Material
    conjuntmaterials* c_materials;

    // Luz
    /*Llum* light;
    vec3 Id, Is, Ia;*/

};


#endif // ESCENA_H
