#ifndef CONJUNTLLUMS_H
#define CONJUNTLLUMS_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

#include<llum.h>

using namespace std;

class conjuntllums
{
    public:
        conjuntllums();
        ~conjuntllums();
        void setAmbientToGPU(QGLShaderProgram *program);
        void addLlum(Llum* l);
        QList<Llum*> getLlum();

    public:
        // Guardamos todas las luces del escenario.
        QList<Llum*> list_llum;
        GLuint ambient;
        vec3 ambientGPU;
};

#endif // CONJUNTLLUMS_H
