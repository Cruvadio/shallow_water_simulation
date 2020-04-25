#version 450

#define IOR_AIR 1.0
#define IOR_WATER 1.33

out vec4 color;

uniform samplerCube skybox;
uniform sampler2D wallTexture;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 fragPos;
in vec3 normal;

vec2 intersectCube(vec3 origin, vec3 ray, vec3 cubeMin, vec3 cubeMax) {
    vec3 tMin = (cubeMin - origin) / ray;
    vec3 tMax = (cubeMax - origin) / ray;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    return vec2(tNear, tFar);
  }

vec3 getWallColor(vec3 point) {
    float scale = 0.5;

    vec3 wallColor;
    vec3 normal;
    if (abs(point.x) > 0.999) {
      wallColor = texture2D(wallTexture, point.yz * 0.5 + vec2(1.0, 0.5)).rgb;
      normal = vec3(-point.x, 0.0, 0.0);
    } else if (abs(point.z) > 0.999) {
      wallColor = texture2D(wallTexture, point.yx * 0.5 + vec2(1.0, 0.5)).rgb;
      normal = vec3(0.0, 0.0, -point.z);
    } else {
      wallColor = texture2D(wallTexture, point.xz * 0.5 + 0.5).rgb;
      normal = vec3(0.0, 1.0, 0.0);
    }

    return wallColor;
    }


void main()
{
  const vec3	c1 = vec3 ( 0.0, 0.4, 0.3);
	const vec3	c2 = vec3 ( 0.0, 0.3, 0.7);

  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;
  vec3 N = normalize(normal);
  //if (gl_FrontFacing) N = normalize(normal);
  //else N = -normalize(normal);
  vec3 L = normalize(lightPos - fragPos);
  vec3 V = normalize(cameraPos - fragPos);

  vec3 R = reflect(-V, N);
  vec3 reflectColor = texture(skybox, R).rgb;

  vec3 Rr = refract(-V, N, IOR_AIR/ IOR_WATER);
  vec2 t = intersectCube(fragPos, normalize(Rr), vec3(-1, -1, -1), vec3(1, 2, 1));
  vec3 refractColor = getWallColor(fragPos + Rr * t.y);

  float	ca = dot( V, N);

  vec3 waterColor1 = mix(reflectColor, refractColor, ca);
  vec3 waterColor2 = mix( c1, c2, ca );

  vec3 waterColor = mix( waterColor1, waterColor2,  0.1 );

  vec3 reflectDir = reflect(-L, N);  

  float specularStrength = 2;
  float spec = pow(max(dot(V, reflectDir), 0.0), 32) * (1 - ca);
  vec3 specular = specularStrength * spec * lightColor;  


  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse + specular) * waterColor;

  color = vec4(result, 1.0);
  //color = vec4(1.0);
}