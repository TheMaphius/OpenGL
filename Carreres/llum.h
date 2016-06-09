#ifndef LLUM_H
#define LLUM_H

#include <stdio.h>
#include <Common.h>
#include <QGLShaderProgram>

using namespace std;

class Llum{

    public:
        Llum();
        ~Llum();
        void toGPU(QGLShaderProgram *program);
        void initialize(QGLShaderProgram *program);
        void LlumPuntual(vec4 position, vec3 Id, vec3 Is, vec3 Ia);
        void LlumDirectional(vec4 direction, vec3 Id, vec3 Is, vec3 Ia);
        void LlumSpotLight(vec4 direction, float angle, vec3 Id, vec3 Is, vec3 Ia);
        void setCoeficients(float coef_a, float coef_b, float coef_c);
        void setPosition(vec4 position);

        vec4 positionLight;
        vec4 directionLight;

        vec3 Id;            // Intensidad difusa
        vec3 Is;            // Intensidad especular
        vec3 Ia;            // Intensidad ambiente

        float coef_a;       // Constant
        float coef_b;       // Lineal
        float coef_c;       // Cuadrática

        float angle;        // angulo de obertura

        struct {
            GLuint posicion;

            GLuint a;
            GLuint b;
            GLuint c;

            GLuint Id;    // Luz difusa
            GLuint Is;    // Luz Especular
            GLuint Ia;    // Luz ambiente

        } gl_IdLlum;

};

#endif // LLUM_H
