#version 450

layout(location = 0) in vec3 aVertex;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;

void main(void)
{
  fragPos      = vec3(model * vec4(aVertex, 1.0));
  normal       = normalize(mat3(transpose(inverse(model))) * aNormal); 
  gl_Position  = projection * view * model * vec4(aVertex, 1.0);
}
