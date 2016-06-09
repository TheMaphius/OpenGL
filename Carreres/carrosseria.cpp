#include "carrosseria.h"

Carrosseria::Carrosseria() : Objecte(NumVerticesF)
{
    light = new Llum();

    Is = vec3(0.75, .2, .2); Id = vec3(1,1,1); Ia = vec3(.2, .2, .2);
    light->LlumPuntual(vec4(0,5,0), Id, Is, Ia);
    light->setCoeficients(.25,.25,.25);

    Ks = vec3(0.727811, 0.626959, 0.626959); Kd = vec3(0.61424, 0.04136, 0.04136); Ka = vec3(0.1745, 0.01175, 0.01175);
    float beta = .6;
    material = new Material("coche", Kd, Ks, Ka, beta);
}

Material* Carrosseria::getMaterial(){
    return material;
}

Llum* Carrosseria::getLlum(){
    return light;
}

void Carrosseria::doGouraud(){
    this->gouraud();
}
