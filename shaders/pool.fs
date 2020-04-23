#version 450

out vec4 color;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

in vec3 fragPos;
in vec3 normal;

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

  float	ca = dot( V, N);
  vec3 waterColor = mix( c1, c2, ca );

  vec3 reflectDir = reflect(-L, N);  

  float specularStrength = 2;
  float spec = pow(max(dot(V, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;  


  float diff = max(dot(N, L), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse + specular) * waterColor;

  color = vec4(result, 1.0);
  //color = vec4(1.0);
}