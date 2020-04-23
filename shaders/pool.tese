#version 450 core
 
layout(quads, equal_spacing, ccw) in;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 aNormal;

//quad interpol
vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)
{
 vec4 a = mix(v0, v1, gl_TessCoord.x);
 vec4 b = mix(v3, v2, gl_TessCoord.x);
 return mix(a, b, gl_TessCoord.y);
}
 

out vec3 fragPos;
out vec3 normal;

void main()
{ 
  vec4 position = interpolate(
  gl_in[0].gl_Position, 
  gl_in[1].gl_Position, 
  gl_in[2].gl_Position, 
  gl_in[3].gl_Position);

  gl_Position = projection * view * model * position;
  normal = mat3(transpose(inverse(model))) * aNormal; 
  fragPos = vec3(model * position);
}