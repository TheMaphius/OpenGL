#include "conjuntllums.h"

conjuntllums::conjuntllums()
{
    this->ambientGPU = vec3(128, 128, 128);
    this->ambientGPU /= 255;
}

conjuntllums::~conjuntllums()
{
    printf("DESTRUCTOR!");
}

void conjuntllums::setAmbientToGPU(QGLShaderProgram *program){

    for(int i = 0; i < this->list_llum.size(); i++){
        ambient = program->uniformLocation("IaEsc");
        glUniform4fv(ambient, 1, vec4(ambientGPU, 1.0));
        this->list_llum.at(i)->toGPU(program);
    }
}

void conjuntllums::addLlum(Llum* l){
    this->list_llum.append(l);
}

QList<Llum*> conjuntllums::getLlum(){
    return list_llum;
}
