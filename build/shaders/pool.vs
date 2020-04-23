#version 450

layout(location = 0) in vec4 aVertex;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec4 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;

void main(void)
{
  fragPos      = vec3(model * aVertex);
  normal       = mat3(transpose(inverse(model))) * vec3(aNormal); 
  gl_Position  = projection * view * model * aVertex;
  //gl_PointSize = 2;
}
