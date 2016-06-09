#include "material.h"

Material::Material(){

}

Material::Material(string name, vec3 Kd, vec3 Ks, vec3 Ka, float beta){

    this->material = name;
    this->Kd = Kd;
    this->Ks = Ks;
    this->Ka = Ka;
    this->beta = beta;
}


Material::~Material(){

    //DESTRUCTOR
}


void Material::toGPU(QGLShaderProgram *program){
    this->initialize(program);
}

void Material::initialize(QGLShaderProgram *program){

    gl_IdMaterial.Kd = program->uniformLocation("material.Kd");
    gl_IdMaterial.Ks = program->uniformLocation("material.Ks");
    gl_IdMaterial.Ka = program->uniformLocation("material.Ka");
    gl_IdMaterial.beta = program->uniformLocation("material.beta");

    glUniform3fv(gl_IdMaterial.Kd, 1, Kd );         // difusa és un vec3);
    glUniform3fv(gl_IdMaterial.Ks, 1, Ks );      // especular és un vec3);
    glUniform3fv(gl_IdMaterial.Ka, 1, Ka );       // ambbiente és un vec3);
    glUniform1f(gl_IdMaterial.beta, beta);

}

void Material::setComponentMaterials(vec3 Kd, vec3 Ks, vec3 Ka, float beta){

    this->Kd = Kd;
    this->Ks = Ks;
    this->Ka = Ka;
    this->beta = beta;
}

string Material::getMaterialName(){
    return this->material;
}
