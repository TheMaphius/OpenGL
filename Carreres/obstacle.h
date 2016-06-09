#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <objecte.h>
#include <time.h>
#include <roda.h>

class obstacle : public Objecte
{
public:
    obstacle();
    ~obstacle();
    obstacle(QString filename, int num_obst, GLfloat xorig, GLfloat yorig, GLfloat zorig);   
    void toGPU(QGLShaderProgram *p);
    void draw();

private:

    static const int NumVertices = 36; // 6 caras * 2 trinangulos * 3 vertex
    void readObj(QString filename);
    Capsa3D calculCapsa3D();
    void make();  
    void aplicaTG(mat4 m);
    void aplicaTGPoints(mat4 m);
    void aplicaTGCentrat(mat4 m);

public:
    Material* getMaterial();
    Llum* getLlum();

private:

    static const int NumVerticesF = 100000;
    static const int NumCares = 300000;

    Objecte *cono;
    // Guardamos todos los obstaculos.
    QList<Objecte*> list_obstacles;

};

#endif // OBSTACLE_H
