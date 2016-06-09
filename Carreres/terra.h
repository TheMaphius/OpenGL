#ifndef TERRA_H
#define TERRA_H

#include <objecte.h>
#include <llum.h>
#include <conjuntllums.h>

class Terra : public Objecte
{
public:
    Terra();
    ~Terra();
    Terra(float amplaria, float profundidad, float y,
          GLfloat xorig, GLfloat yorig, GLfloat zorig);
    Material* getMaterial();
    Llum* getLlum();

private:

    static const int NumVertices = 6;
    void quad(int a, int b, int c, int d);
    void make();
    float a, p;

    point4 vertices[4]; // 4 vertexs del cub
    color4 vertex_colors[4]; // 4 colors RGBA associats a cada vertex

};

#endif // TERRA_H
