#include "obstacle.h"

obstacle::obstacle() : Objecte(NumVerticesF)
{
}


obstacle::obstacle(QString filename, int num_obst, GLfloat xorig, GLfloat yorig, GLfloat zorig) : Objecte(NumVerticesF)
{

    xorig = 0;
    yorig = 0;
    zorig = 0;

    light = new Llum();

    Is = vec3(1, 0.75, 0.5); Id = vec3(1,1,1); Ia = vec3(1,0.5,0.5);
    light->LlumPuntual(vec4(0,5,0), Id, Is, Ia);
    light->setCoeficients(.25,.25,.25);

    Ks = vec3(0.62828, 0.555802, 0.366065); Kd = vec3(0.75164, 0.60648, 0.22648); Ka = vec3(0.24725, 0.1995, 0.0745);
    material = new Material("obstaculo", Kd, Ks, Ka, 0.4);

    readObj(filename);
    make();
}

obstacle::~obstacle()
{
    // Destructor
}


// Leemos el fichero del obstacle './Dataset/obstacle.obj'
void obstacle::readObj(QString filename)
{

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        //Objecte *cono;

        int vindexAct = 0;
        int vindexUlt = 0;

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4)
                {
                    fprintf (stderr, "Too few coordinates");//: '%s'", str_orig);
                    exit (-1);
                }
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5)
                {
                  QString sw(ReadFile::words[4]);
                  double w = sw.toDouble();
                  x/=w;
                  y/=w;
                  z/=w;
                }
                // S'afegeix el vertex a l'objecte
                cono->vertexs.push_back(point4(x, y, z, 1));
                vindexAct++;

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                construeix_cara (&ReadFile::words[1], nwords-1, cono, vindexUlt);
            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            else if (!strcmp (first_word, "o")) {
                //cada nou objecte s'actualitza aquest Ã­ndex
                vindexUlt = vindexAct;

                /* Objeto partes*/
                cono = NULL;

                if(!strcmp(ReadFile::words[1], "Cone")){
                    cono = new Objecte(NumVerticesF);
                }
            }
        }
    }

}


/* Método calculCapsa3D sobrescrito*/
Capsa3D obstacle::calculCapsa3D(){

    this->capsa = cono->calculCapsa3D();
    return this->capsa;
}


/* Método make sobrescrito*/
void obstacle::make(){

    // Hacemos un make() de cada parte del coche.
    cono->make();

    // Hacemos gouraud para los obstaculos
    cono->gouraud();

    // Calculamos la caja 3D del coche
    calculCapsa3D();

    // Escalamos el obstaculo a escala unitaria.
    GLfloat escala = 1.0 / cono->capsa.unitario;

    float x = rand() % 500 - 250;     // <-- Centro random [-25,25]
    float z = rand() % 500 - 250;
    float rscale = rand() % 5 + 1;     // <-- Escale random [1,10]

    x /= 10;
    z /= 10;

    //cout<<"x: "<<x<<" z: "<<z<<" escale: "<<rscale<<endl;

    // Operacion de der-->izq:
    //  o) Trasladar al origen.
    //  o) Escalado unitario.
    //  o) Escalado del User.
    //  o) Devolver el objeto.

    mat4 m = Translate(x, rscale/2.5, z)
            *Scale(rscale,rscale,rscale)
            *Scale(escala, escala, escala)
            *Translate(-cono->capsa.centro);

    // Aplicamos las transformaciones y recalculamos la nueva caja 3D
    aplicaTG(m);
    calculCapsa3D();
}

/* Método toGPU sobrescrito*/
void obstacle::toGPU(QGLShaderProgram *p){
    cono->toGPU(p);
}

/* Método draw sobrescrito*/
void obstacle::draw(){
    cono->draw();
}

/* Método aplicaTG sobrescrito*/
void obstacle::aplicaTG(mat4 m){
    cono->aplicaTG(m);
}

/* Método aplicaTGPoints sobrescrito*/
void obstacle::aplicaTGPoints(mat4 m){
    cono->aplicaTGPoints(m);
}

/* Método aplicaTGCentrat sobrescrito*/
void obstacle::aplicaTGCentrat(mat4 m){
    cono->aplicaTGCentrat(m);
}

Material* obstacle::getMaterial(){
    return material;
}

Llum* obstacle::getLlum(){
    return light;
}
