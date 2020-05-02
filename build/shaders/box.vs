#version 450

layout(location = 0) in vec3 aVertex;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform sampler2D normalMap;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main(void)
{
  float scale = 0.1;

  fragPos      = aVertex;
  if (dot(aNormal, vec3(0, 1.0, 0)) != 0)
  {
    normal       = mat3(transpose(inverse(model))) * (2.0 * texture(normalMap, texCoord).rgb - vec3(1.0));
  } 
  else
    normal       = mat3(transpose(inverse(model))) * aNormal;
  texCoord     = aTexCoord;
  gl_Position  = projection * view * model * vec4(fragPos, 1.0);

  fragPos      = vec3(model * vec4(aVertex, 1.0)); 
  //gl_PointSize = 2;
}
