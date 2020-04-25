#version 450

out vec4 color;
uniform sampler2D wallTexture;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;


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

void main ()
{
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

  vec3 result = (ambient + diffuse + specular) * texture(wallTexture, texCoord).rgb;

  color = vec4(result, 1.0);
  color = vec4(getWallColor(fragPos), 1.0);
}