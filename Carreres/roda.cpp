#include "roda.h"

Roda::Roda() : Objecte(NumVerticesF)
{
    //this->gourard = true;

    light = new Llum();

    Is = vec3(0, 0, 0); Id = vec3(1,1,1); Ia = vec3(1,0.5,0.5);
    light->LlumPuntual(vec4(0,5,0), Id, Is, Ia);
    light->setCoeficients(.25,.25,.25);

    // Pearl
    //Ks = vec3(0.296648, 0.296648, 0.296648); Kd = vec3(1, 0.829, 0.829); Ka = vec3(0.25, 0.20725, 0.20725);
    //float beta = .088;
    // Black rubber
    //Ks = vec3(.4, .4, .4); Kd = vec3(.01, .01, .01); Ka = vec3(.02, .02, .02);
    //float beta = .078125;
    // Obsidian
    Ka = vec3(0.05375, 0.05, 0.06625); 	Kd = vec3(0.18275, 0.17, 0.22525); 	Ks = vec3(0.332741, 0.328634, 0.346435);
    float beta = .3;
    material = new Material("coche", Kd, Ks, Ka, beta);
}

Material* Roda::getMaterial(){
    return material;
}

Llum* Roda::getLlum(){
    return light;
}

void Roda::doGouraud(){
    this->gouraud();
}
