#ifndef COTXE_H
#define COTXE_H

#include <objecte.h>
#include <roda.h>
#include <carrosseria.h>
#include <material.h>

#include <iostream>

using namespace std;

typedef enum {FORWARD = 0, BACKWARD = 1, TURNLEFT = 2, TURNRIGHT = 3} TipDir;

class Cotxe: public Objecte
{
public:
    Cotxe(QString n);
    Cotxe(QString n, GLfloat tamanio, GLfloat x0, GLfloat y0, GLfloat z0,
          double girx, double giry, double girz,
          float xdir, float ydir, float zdir);

    void readObj(QString filename);
    Capsa3D calculCapsa3D();
    void make();
    void toGPU(QGLShaderProgram *p);
    void draw();
    void aplicaTG(mat4 m);
    void aplicaTGPoints(mat4 m);
    void aplicaTGCentrat(mat4 m);
    void forward();
    void backward();
    void turnleft();
    void turnright();
    void RotarRuedas(mat4 m);
    void getCarroceriatoGPU(QGLShaderProgram *p);
    void getRodatoGPU(QGLShaderProgram *p);

    // Methods Material
    Material* getMaterial();
    Material* getMaterialCarroceria();
    Material* getMaterialRoda();

    // Methods Light
    Llum* getLight();
    Llum* getLlumCarroceria();
    Llum* getLlumRoda();

    void doGouraud();

    point4 direction;
    // Ángulo de girado
    GLint giro;
    // Ámgulo máx de ruedas.
    GLint angulo_rueda;
    // Speed
    GLfloat speed;
    // Scala
    GLfloat scale;
    // Direction
    TipDir dir;

    // Guardamos todas las partes del coche.
    QList<Objecte*> partes_coche;

private:

    static const int NumVerticesF = 100000;
    static const int NumCares = 300000;

    // Definimos las ruedas del coche.
    Roda *DelanteraD;
    Roda *DelanteraI;
    Roda *TraseraD;
    Roda *TraseraI;

    // Definimos el chasis del coche.
    Carrosseria *chasis;

    // Materials
    vec3 Kd, Ks, Ka;
    Material* material;

};

#endif // COTXE_H
