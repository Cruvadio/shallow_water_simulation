#version 450

out vec4 color;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 fragPos;
in vec3 normal;

void main()
{
  vec3 objectColor = vec3(0.4745, 0.3098, 0.0);
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;
  vec3 N = normalize(normal);
  if (gl_FrontFacing) N = normalize(normal);
  else N = -normalize(normal);
  vec3 L = normalize(lightPos - fragPos);
  vec3 V = normalize(cameraPos - fragPos);
  vec3 reflectDir = reflect(-L, N);  

  float specularStrength = 2;
  float spec = pow(max(dot(V, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;  


  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;

  color = vec4(result, 1.0f);
}