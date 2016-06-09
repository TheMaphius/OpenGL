#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

using namespace std;

class Material{

    public:
        Material();
        Material(string material, vec3 Kd, vec3 Ks, vec3 Ka, float beta);
        ~Material();
        void toGPU(QGLShaderProgram *program);
        void initialize(QGLShaderProgram *program);
        void setComponentMaterials(vec3 Kd, vec3 Ks, vec3 Ka, float beta);
        string getMaterialName();

    private:

        string material;    // Nombre material

        vec3 Kd;            //Componente material difusa
        vec3 Ks;            //Componente material especular
        vec3 Ka;            //Componente material ambiente

        float beta;      // Coeficiente reflexion especular

        struct {
            GLuint Kd;      // Componente difusa
            GLuint Ks;      // Componente Especular
            GLuint Ka;      // Componente ambiente
            float beta;
        } gl_IdMaterial;

};

#endif // MATERIAL_H
