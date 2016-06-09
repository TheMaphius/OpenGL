#include "terra.h"

Terra::Terra() : Objecte(NumVertices)
{
}

Terra::~Terra()
{
}

Terra::Terra(float amplaria, float profundidad, float y, GLfloat xorig,
             GLfloat yorig, GLfloat zorig) : Objecte(NumVertices)
{
    a = amplaria; p = profundidad;
    Index = 0;

    /* Ponemos los vertices en el plano donde yorig será 0*/
    /* Los ejes x serán la mitad de la anchura y para z lo mismo con profundidad*/
    vertices[0] = point4( xorig - a/2, y, zorig - p/2, 1);
    vertices[1] = point4( xorig - a/2, y, zorig + p/2, 1);
    vertices[2] = point4( xorig + a/2, y, zorig - p/2, 1);
    vertices[3] = point4( xorig + a/2, y, zorig + p/2, 1);


    // RGBA colors
    vertex_colors[0] = color4( 0.0, 0.0, 0.0, 1.0 );  // black
    vertex_colors[1] = color4( 1.0, 0.0, 0.0, 1.0 );  // red
    vertex_colors[2] = color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
    vertex_colors[3] = color4( 0.0, 1.0, 0.0, 1.0 );  // green

    light = new Llum();

    Is = vec3(1, 1, 1); Id = vec3(1,1,1); Ia = vec3(1,0.5,0.5);
    light->LlumPuntual(vec4(0,5,0), Id, Is, Ia);
    light->setCoeficients(.25,.25,.25);

    Ks = vec3(0.633, 0.727811, 0.633); Kd = vec3(0.07568, 0.61424, 0.07568); Ka = vec3(0.0215, 0.1745, 0.0215);
    float beta = .6;
    material = new Material("terra", Kd, Ks, Ka, beta);

    /* Montamos el cuadrado siguiendo el orden inverso a las agujas del reloj*/
    quad(0,1,3,2);

    this->calculCapsa3D();

}

/* Función para crear el cuadrado mediante dos triangulos*/
void Terra::quad(int a, int b, int c, int d)
{
    normal[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    normal[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    normal[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    normal[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    normal[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    normal[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

void Terra::make(){

    Index = 0;
    quad(0,1,3,2);
    this->calculCapsa3D();

}

Material* Terra::getMaterial(){
    return material;
}

Llum* Terra::getLlum(){
    return light;
}
