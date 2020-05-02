#version 450

#define IOR_AIR 1.0
#define IOR_WATER 1.33


out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;


layout(std430, binding = 1) buffer CurrentHeights
{
    float Hcurr[];
};

uniform int width;
uniform int height;

uniform sampler2D wallTexture;
uniform sampler2D normalMap;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform vec3 sphereCenter;
uniform float sphereRadius;

vec3 waterColor = vec3(0.8627, 1.0, 0.8745);


float intersectSphere(vec3 origin, vec3 ray, vec3 sphereCenter, float sphereRadius) {
    vec3 toSphere = origin - sphereCenter;
    float a = dot(ray, ray);
    float b = 2.0 * dot(toSphere, ray);
    float c = dot(toSphere, toSphere) - sphereRadius * sphereRadius;
    float discriminant = b*b - 4.0*a*c;
    if (discriminant > 0.0) {
      float t = (-b - sqrt(discriminant)) / (2.0 * a);
      if (t > 0.0) return t;
    }
    return 1.0e6;
  }


vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax) {
    vec3 tMin = (cubeMin - origin) / ray;
    vec3 tMax = (cubeMax - origin) / ray;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    return vec2(tNear, tFar);
  }

vec3 getWallColor(vec3 point,in vec3 L, int index) {
    float scale = 0.5;

    vec3 wallColor;
    vec3 normal;
    if (abs(point.x) > 0.999) {
      wallColor = texture2D(wallTexture, point.yz * 0.5 + vec2(1.0, 0.5)).rgb;
      normal = vec3(point.x, 0.0, 0.0);
    } else if (abs(point.z) > 0.999) {
      wallColor = texture2D(wallTexture, point.yx * 0.5 + vec2(1.0, 0.5)).rgb;
      normal = vec3(0.0, 0.0, point.z);
    } else {
      wallColor = texture2D(wallTexture, point.xz * 0.5 + 0.5).rgb;
      normal = 2 * texture2D(normalMap, point.xz * 0.5 + 0.5).rgb - vec3(1.0);
    }

    scale /= length(point);

    scale *= 1.0 - 0.9 / pow(length(point - sphereCenter) / sphereRadius, 4.0);

    vec3 refractedLight = -refract(-L, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    float diffuse = max(0.0, dot(refractedLight, normal));
    /*if (point.y > Hcurr[index])
    {
        vec2 t = intersectCube(point, refractedLight, vec3(-1.0, -1.0, -1.0), vec3(1.0, 2.0, 1.0));
        diffuse *= 1.0 / (1.0 + exp(-200.0 / (1.0 + 10.0 * (t.y - t.x)) * (point.y + refractedLight.y * t.y - 2.0 / 12.0)));
    }*/
    scale += diffuse * 0.5;
    return wallColor * scale;
    }

vec3 getSphereColor (vec3 point, vec3 L)
{
    vec3 color = vec3(0.2);

    color *= 1.0 - 0.9 / pow((1.0 + sphereRadius - abs(point.x)) / sphereRadius, 3.0);
    color *= 1.0 - 0.9 / pow((1.0 + sphereRadius - abs(point.z)) / sphereRadius, 3.0);
    color *= 1.0 - 0.9 / pow((point.y + 1.0 + sphereRadius) / sphereRadius, 3.0);

    vec3 sphereNormal = (point - sphereCenter) / sphereRadius;
    vec3 refractedLight = refract(-L, vec3(0.0, 1.0, 0.0), IOR_AIR / IOR_WATER);
    float diffuse = max(0.0, dot(-refractedLight, sphereNormal)) * 0.5;

    color += diffuse;

    return color;
}

int countIndex( vec3 fragPos)
{
    int i = width*(1 - int(0.5 *(fragPos.x + 1.)));
    int j = height * int(0.5 * (1. + fragPos.z));
    return i + j * width;
}


void main ()
{
  /*
    float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  vec3 N = normalize(normal);;
  vec3 L = normalize(lightPos - fragPos);
  vec3 V = normalize(cameraPos - fragPos);

  vec3 reflectDir = reflect(-L, N);  

  float specularStrength = 2;
  float spec = pow(max(dot(V, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;  


  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse + specular) * texture(wallTexture, texCoord).rgb;*/
  vec3 L = normalize(lightPos - fragPos);
  vec3 V = normalize(cameraPos - gl_FragCoord.xyz);

  int index = countIndex(fragPos);
  vec3 result;
  /*float d = intersectSphere(cameraPos, V, sphereCenter, sphereRadius);
  if (d < 1.0e6)
    result = getSphereColor(cameraPos + d * V, L);
  else*/
    result = getWallColor(fragPos, L, index);

  
  if (fragPos.y < Hcurr[index])
    result *= waterColor;
  color = vec4(result, 1.0);

}