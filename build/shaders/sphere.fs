#version 450

#define IOR_AIR 1.0
#define IOR_WATER 1.33


layout(std430, binding = 1) buffer CurrentHeights
{
    float Hcurr[];
};

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 lightColor;

uniform vec3 sphereCenter;
uniform float sphereRadius;

uniform int width;
uniform int height;

vec3 waterColor = vec3(0.8627, 1.0, 0.8745);


in vec3 fragPos;
in vec3 normal;

vec3 getSphereColor (vec3 point, vec3 L)
{
    vec3 color = vec3(0.2);

    color *= 1.0 - 0.9 / pow((1.0 + sphereRadius - abs(point.x)) / sphereRadius, 3.0);
    color *= 1.0 - 0.9 / pow((1.0 + sphereRadius - abs(point.z)) / sphereRadius, 3.0);
    color *= 1.0 - 0.9 / pow((point.y + 1.0 + sphereRadius) / sphereRadius, 3.0);

    vec3 refractedLight = refract(-L, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    float diffuse = max(0.0, dot(-refractedLight, normal)) * 0.5;

    color += diffuse;

    return color;
}

int countIndex( vec3 fragPos)
{
    int i = width*(1 - int(0.5 *(fragPos.x + 1.)));
    int j = height * int(0.5 * (1. + fragPos.z));
    return i + j * width;
}

out vec4 color;

uniform samplerCube skybox;


void main()
{

  vec3 L = normalize(lightPos - fragPos);

  int index = countIndex(fragPos);
  vec3 result;
    result = getSphereColor(fragPos, L);

  
  if (fragPos.y < Hcurr[index])
    result *= waterColor;
  color = vec4(result, 1.0);
}