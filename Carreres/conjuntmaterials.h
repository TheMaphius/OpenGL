#ifndef CONJUNTMATERIALS_H
#define CONJUNTMATERIALS_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

#include<material.h>

using namespace std;

class conjuntmaterials
{
    public:
        conjuntmaterials();
        ~conjuntmaterials();
        void setAmbientToGPU(QGLShaderProgram *program);
        void addMaterial(Material* m);
        QList<Material*> getMaterial();

    public:
        // Guardamos todos los materiales del escenario.
        QList<Material*> list_material;
    };

#endif // CONJUNTMATERIALS_H
