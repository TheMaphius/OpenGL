#include "llum.h"

Llum::Llum(){

}

/*Llum::Llum(vec4 position, vec3 Id, vec3 Is, vec3 Ia){

    this->posicionLuz = position;
    this->difusa = Id;
    this->especular = Is;
    this->ambiente = Ia;
}*/


Llum::~Llum(){

    //DESTRUCTOR
}


void Llum::toGPU(QGLShaderProgram *program){
    this->initialize(program);
}

void Llum::initialize(QGLShaderProgram *program){

    gl_IdLlum.posicion = program->uniformLocation("light.LightPosition");

    gl_IdLlum.a = program->uniformLocation("light.coef_a");
    gl_IdLlum.b = program->uniformLocation("light.coef_b");
    gl_IdLlum.c = program->uniformLocation("light.coef_c");

    gl_IdLlum.Id = program->uniformLocation("light.Ld");
    gl_IdLlum.Is = program->uniformLocation("light.Ls");
    gl_IdLlum.Ia = program->uniformLocation("light.La");

    // Bind de les zones de memoria que corresponen a les variables de la CPU
    glUniform4fv(gl_IdLlum.posicion, 1, this->positionLight); //posicioLlum és un vec4

    glUniform1f(gl_IdLlum.a, this->coef_a);               // coef_a és un Glfloat
    glUniform1f(gl_IdLlum.b, this->coef_b);               // coef_b és un Glfloat
    glUniform1f(gl_IdLlum.c, this->coef_c);               // coef_c és un Glfloat

    glUniform3fv(gl_IdLlum.Id, 1, this->Id );         // difusa és un vec3);
    glUniform3fv(gl_IdLlum.Is, 1, this->Is );      // especular és un vec3);
    glUniform3fv(gl_IdLlum.Ia, 1, this->Ia );       // ambbiente és un vec3);

}

void Llum::LlumPuntual(vec4 position, vec3 Id, vec3 Is, vec3 Ia){

    this->positionLight = normalize(position);
    this->Id = Id;
    this->Is = Is;
    this->Ia = Ia;

}

void Llum::LlumDirectional(vec4 direction, vec3 Id, vec3 Is, vec3 Ia){

    this->directionLight = normalize(direction);
    this->Id = Id;
    this->Is = Is;
    this->Ia = Ia;
}

void Llum::LlumSpotLight(vec4 direction, float angle, vec3 Id, vec3 Is, vec3 Ia){

    this->directionLight = normalize(direction);
    this->angle = angle;
    this->Id = Id;
    this->Is = Is;
    this->Ia = Ia;
}

void Llum::setCoeficients(float coef_a, float coef_b, float coef_c){

    this->coef_a = coef_a;
    this->coef_b = coef_b;
    this->coef_c = coef_c;
}

void Llum::setPosition(vec4 position){

    this->positionLight = position;
}
