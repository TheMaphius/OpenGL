#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


IN vec4 vPosition;
IN vec4 vNormal;

// Salidas al fragment

OUT vec3 n;
OUT vec3 l;
OUT vec3 h;

OUT vec3 Ia;

OUT float p;

OUT vec3 Ks;
OUT vec3 Kd;
OUT vec3 Ka;

OUT float beta;

OUT vec4 color;

struct tipusLlum{

    vec4 LightPosition;

    float coef_a;
    float coef_b;
    float coef_c;

    vec3 Ld;    // Luz difusa
    vec3 Ls;    // Luz Especular
    vec3 La;    // Luz ambiente
};

struct tipusMaterial{
    vec3 Kd;    // Componente difusa
    vec3 Ks;    // Componente Especular
    vec3 Ka;    // Componente ambiente
    float beta;
};

// Añadimos la matriz modelview y projection
uniform mat4 model_view;
uniform mat4 projection;

//Añadimos la estructura del tipo de luz
uniform tipusLlum light;
//Añadimos la estructura del tipo material
uniform tipusMaterial material;

// Añadimos la luz ambiente
uniform vec3 ambient;

void main() 
{
    // Calculamos las transformaciones en cada punto con las matrices MV y proyection
    gl_Position = projection * model_view * vPosition;
    gl_Position /= gl_Position.w;   // <-- Dividimos los puntos calculados por la componente homogenea
                                    //     del vec4

    n = vNormal.xyz;
    l = normalize(vec3(light.LightPosition.x - vPosition.x, light.LightPosition.y - vPosition.y, light.LightPosition.z - vPosition.z));
    h = normalize(normalize(-vPosition.xyz) + l);

    float d = length(light.LightPosition.xyz - vPosition.xyz);

    Ia = (material.Ka * ambient);

    p = (1.0 / (((light.coef_a)*(d*d)) + (light.coef_b*d) + light.coef_a));

    Kd = (light.Ld * material.Kd) ;
    Ks = (light.Ls * material.Ks);
    Ka = (light.La * material.Ka);

    beta = material.beta;

}


