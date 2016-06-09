#include "conjuntmaterials.h"

conjuntmaterials::conjuntmaterials()
{
}

conjuntmaterials::~conjuntmaterials()
{
    printf("DESTRUCTOR!");
}

void conjuntmaterials::setAmbientToGPU(QGLShaderProgram *program){


}

void conjuntmaterials::addMaterial(Material* m){

    bool exist = false;

    for(int i = 0; i < this->list_material.size(); i++)
        if(m->getMaterialName() == this->list_material.at(i)->getMaterialName())
            exist = true;

    if(!exist)
        this->list_material.append(m);
}

QList<Material*> conjuntmaterials::getMaterial(){
    return list_material;
}
