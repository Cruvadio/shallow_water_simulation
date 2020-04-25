#version 450

layout(location = 0) in vec3 aVertex;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main(void)
{
  fragPos      = aVertex;
  fragPos.y    = (fragPos.y* (7.0 / 12.0) - 1.0);
  normal       = mat3(transpose(inverse(model))) * ( - aNormal); 
  texCoord     = aTexCoord;
  gl_Position  = projection * view * model * vec4(fragPos, 1.0);

  fragPos      = vec3(model * vec4(aVertex, 1.0)); 
  //gl_PointSize = 2;
}
