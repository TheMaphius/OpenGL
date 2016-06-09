#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif


IN vec4 vPosition;
IN vec4 vNormal;

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

uniform vec3 ambient;

void main() 
{
  // Calculamos las transformaciones en cada punto con las matrices MV y proyection
  /*gl_Position = projection * model_view * vPosition;
  gl_Position /= gl_Position.w;   // <-- Dividimos los puntos calculados por la componente homogenea
                                  //     del vec4

  vec3 n = normalize(vec3(vNormal.x, vNormal.y, vNormal.z));
  vec3 l = normalize(vec3(light.LightPosition.x - vPosition.x, light.LightPosition.y - vPosition.y, light.LightPosition.z - vPosition.z));
  vec3 h = normalize(normalize(-vPosition.xyz) + l);

  float d = length(light.LightPosition.xyz - vPosition.xyz);

  float p = (1.0 / (((light.coef_a)*(d*d)) + (light.coef_b*d) + light.coef_a));
  vec3 I = (material.Ka * ambient) + p*(((light.Ld * material.Kd) * max(dot(l, n), 0.0)) + (light.Ls * material.Ks*max(pow(dot(n, h), material.beta), 0.0)) + light.La * material.Ka);
*/

    gl_Position = projection * model_view * vPosition;
    gl_Position /= gl_Position.w;   // <-- Dividimos los puntos calculados por la componente homogenea
                                //     del vec4

    vec3 n = normalize(vNormal.xyz);
    vec3 l = normalize(vec3(light.LightPosition.x - vPosition.x, light.LightPosition.y - vPosition.y, light.LightPosition.z - vPosition.z));
    vec3 h = normalize(normalize(-vPosition.xyz) + l);

    float d = length(light.LightPosition.xyz - vPosition.xyz);

    vec3 Ia = (material.Ka * ambient);

    float p = (1.0 / (((light.coef_a)*(d*d)) + (light.coef_b*d) + light.coef_a));

    vec3 Kd = (light.Ld * material.Kd) ;
    vec3 Ks = (light.Ls * material.Ks);
    vec3 Ka = (light.La * material.Ka);

    float beta = material.beta;

    vec3 I = Ia + p * ((Kd * max(dot(l, n), 0.0))) + ((Ks * max(pow(dot(n, h), beta), 0.0))) + Ka;


    color = vec4(I, 1.0);

} 


