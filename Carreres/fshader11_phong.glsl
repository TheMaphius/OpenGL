#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif

IN vec3 n;
IN vec3 l;
IN vec3 h;

IN vec3 Ia;

IN float p;

IN vec3 Ks;
IN vec3 Kd;
IN vec3 Ka;

IN float beta;

void main()
{
    vec3 vNormal = normalize(n);
    vec3 I = Ia + p * ((Kd * max(dot(l, vNormal), 0.0))) + ((Ks * max(pow(dot(vNormal, h), beta), 0.0))) + Ka;
    gl_FragColor = vec4(I, 1.0);
}
