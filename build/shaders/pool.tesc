#version 450 core


uniform int inner;
uniform int outer;

layout(vertices = 4) out;

void main(void)
{
	gl_TessLevelInner [0] = inner;
	gl_TessLevelInner [1] = inner;
	gl_TessLevelOuter [0] = outer;
	gl_TessLevelOuter [1] = outer;
	gl_TessLevelOuter [2] = outer;
	gl_TessLevelOuter [3] = outer;

	gl_out [gl_InvocationID].gl_Position = gl_in [gl_InvocationID].gl_Position;
}