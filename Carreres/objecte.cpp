#include "objecte.h"

Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints) ,
    QObject(parent)
{
    points = new point4[npoints];
    //colors = new color4[npoints];
    normal = new point4[npoints];
    back = new point4[npoints];
}

Objecte::Objecte(int npoints, QString n, GLdouble tamanio, GLdouble x0, GLdouble y0, GLdouble z0, double girx, double giry, double girz) : numPoints(npoints)
{
    points = new point4[npoints];  
    //colors = new color4[npoints];
    normal = new point4[npoints];
    back = new point4[npoints];

    tam = tamanio;
    std::cout<<"Estic en el constructor parametritzat del cotxe\n";
    xorig = x0;
    yorig = y0;
    zorig = z0;

    xRot = girx;
    yRot = giry;
    zRot = girz;


    nom = n;
    Index = 0;

    //readObj(n);
    //make();

}


Objecte::~Objecte()
{
    delete points;
    //delete colors;
    delete normal;
}


Capsa3D Objecte::calculCapsa3D(){


    vec3    pmin, pmax;

    //Inicializamos los puntos minimos y max de {x,y,z} con los puntos de la escena
    pmin.x = points[0].x;
    pmax.x = points[0].x;
    pmin.y = points[0].y;
    pmax.y = points[0].y;
    pmin.z = points[0].z;
    pmax.z = points[0].z;

    // Recorremos cada punto por cada vertice encontrado y miramos si es max o min en {x,y,z}
    for(int i = 0; i < Index; i++){

        if(pmin.x > points[i].x)    pmin.x = points[i].x;
        if(pmin.y > points[i].y)    pmin.y = points[i].y;
        if(pmin.z > points[i].z)    pmin.z = points[i].z;
        if(pmax.x < points[i].x)    pmax.x = points[i].x;
        if(pmax.y < points[i].y)    pmax.y = points[i].y;
        if(pmax.z < points[i].z)    pmax.z = points[i].z;

        // DEBUG
        //fprintf (stderr, "PMIN: '%f' '%f' '%f'\n", points[i].x, points[i].y, points[i].z);

    }

    // Actualizamos los valores de la Capsa3D {pmin, a,h,p} <-- Struct
    capsa.pmin = pmin;
    capsa.pmax = pmax;
    capsa.a = pmax[0] - pmin[0];    //Calculamos la anchura a partir del xmax y xmin.
    capsa.h = pmax[1] - pmin[1];    //Calculamos la altura a partir del ymax y ymin.
    capsa.p = pmax[2] - pmin[2];    //Calculamos la profundidad a partir del zmax y zmin.

    // DEBUG
    //cout<<"\nDEBUG\nAnchura: "<<capsa.a<<"\nAltura: "<<capsa.h<<"\nProfundidad: "<<capsa.p<<"\nPMIN: "<<capsa.pmin<<"\nPMAX: "<<capsa.pmax<<endl;

    // Calculamos el centro de la caja
    capsa.centro.x = (capsa.pmin.x + capsa.a / 2);
    capsa.centro.y = (capsa.pmin.y + capsa.h / 2);
    capsa.centro.z = (capsa.pmin.z + capsa.p / 2);

    // DEBUG
    //cout<<"\nCENTER\ncX: "<<capsa.centro.x<<"\ncY: "<<capsa.centro.y<<"\ncZ: "<<capsa.centro.z<<endl;

    // Calculamos el lado más grande para poder escalar
    capsa.unitario = max(capsa.a, capsa.h);
    capsa.unitario = max(capsa.unitario, capsa.p);

    // DEBUG
    //cout<<"\nUNITARIO\nValor a Escalar: "<<capsa.unitario<<endl;

    return capsa;
}



void Objecte::aplicaTG(mat4 m)
{
    aplicaTGPoints(m);

    // Actualitzacio del vertex array per a preparar per pintar
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index,
                     &points[0] );

}


void Objecte::aplicaTGPoints(mat4 m)
{
    point4  *transformed_points = new point4[Index];

    for ( int i = 0; i < Index; ++i ) {
        transformed_points[i] = m * points[i];
    }

    transformed_points = &transformed_points[0];
    points = &points[0];

    for ( int i = 0; i < Index; ++i )
    {
        points[i] = transformed_points[i];
    }

    delete transformed_points;
}

void Objecte::aplicaTGCentrat(mat4 m)
{

    // Calcularemos la nueva Capsa3D por cada operación que se haga sobre la escena.
    Capsa3D capsa = calculCapsa3D();

    mat4 op = Translate(capsa.centro) * m * Translate(-capsa.centro);
    aplicaTG(op);

}

void Objecte::toGPU(QGLShaderProgram *pr){

    program = pr;

    std::cout<<"Passo les dades de l'objecte a la GPU\n";

    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4) * Index + sizeof(normal4) * Index,
                  NULL, GL_STATIC_DRAW );

    program->bind();
    glEnable( GL_DEPTH_TEST );

}

// Pintat en la GPU.
void Objecte::draw()
{

    // cal activar el buffer de l'objecte. Potser que ja n'hi hagi un altre actiu
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // per si han canviat les coordenades dels punts
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * Index, sizeof(point4) * Index, &normal[0]);

    // Per a conservar el buffer
    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vNormal");

    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4) * Index, 4);


    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    // Abans nomes es feia: glDrawArrays( GL_TRIANGLES, 0, NumVerticesP );
}

void Objecte::make()
{

    Index = 0;

    for(unsigned int i=0; i<cares.size(); i++)
    {
        cares[i].calculaNormal(vertexs);

        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++)
        {
            points[Index] = vertexs[cares[i].idxVertices[j]];
            //colors[Index] = base_colors[i%4];
            back[cares[i].idxVertices[j]] += cares[i].normal;
            normal[Index] = cares[i].normal;
            Index++;
        }
    }

}


void Objecte::gouraud(){

    Index = 0;

    for(int i = 0; i < cares.size(); i++){

        for(int j = 0; j < cares[i].idxVertices.size(); j++){
            vec4 n = vec4(0.0, 0.0, 0.0, 1.0);
            n = back[cares[i].idxVertices[j]];
            normalize (n);
            normal[Index] = n;
            Index++;
        }
    }

}

float Objecte::getYOrig() {
    return this->yorig;
}


// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), tambe es llegeixen.
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
//

void Objecte::readObj(QString filename)
{

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

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
                vertexs.push_back(point4(x, y, z, 1));
                vindexAct++;

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                construeix_cara (&ReadFile::words[1], nwords-1, this, vindexUlt);
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
            /******else if (!strcmp (first_word, "o")) {
                //cada nou objecte s'actualitza aquest Ã­ndex
                vindexUlt = vindexAct;
            }*/
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }

            //free(words);
        }
    }

}


void Objecte::construeix_cara ( char **words, int nwords, Objecte*objActual, int vindexUlt)
{
    Cara face;
    for (int i = 0; i < nwords; i++)
    {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9'))
        {
            ReadFile::get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1 - vindexUlt);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(objActual->vertexs.size() + vindex - vindexUlt);
            else
            {
                fprintf (stderr, "Zero indices not allowed");//: '%s'\n", str_orig);
                exit (-1);
            }
        }
    }
    face.color = vec4(1.0, 0.0, 0.0, 1.0);
    objActual->cares.push_back(face);
}
