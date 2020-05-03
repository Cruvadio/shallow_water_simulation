#include "common.h"
#include <string.h>

#include "WaterComputeShaderProgram.h"



void WaterComputeShaderProgram::createBuffers()
{
	GLfloat * data = new GLfloat[width * height];
	for (int i = 0; i < width * height; i++)
	{
		data[i] = 0.0;
	}
    glGenBuffers(1, &SSBO_A);
  	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_A);
  	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &SSBO_B);
  	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_B);
  	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &SSBO_C);
  	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_C);
  	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(GLfloat), data, GL_DYNAMIC_DRAW);  
	
	bindBufferBases();
	delete[] data;
}


void WaterComputeShaderProgram::initializeBuffer()
{
	vec4 *positions = new vec4[width * height];
	vec4 *normals = new vec4[width * height];
	vec4 *texCoords = new vec4[width * height];

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			positions[i + width * j].x = (0.5 - (float)(width - i)/(float)width) * 2;
			positions[i + width * j].y = (0.5 - (float)j/(float)height) * 2;
			positions[i + width * j].z = 0;
			positions[i + width * j].w = 1.0;
			normals[i + width * j].x = 0;
			normals[i + width * j].y = 0;
			normals[i + width * j].z = -10.0/(float) width;
			normals[i + width * j].w = 1.0;
			texCoords[i + width * j] = vec4(0.0);
		} 
	glGenBuffers(1, &SSBO_V);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_V);
	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(vec4), positions, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, SSBO_V);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &SSBO_N);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_N);
	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(vec4), normals, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, SSBO_N);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &SSBO_T);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_T);
	glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(vec4), texCoords, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	delete[] positions;
	delete[] normals;
	delete[] texCoords;
}

void WaterComputeShaderProgram::startComputing()
{
	glDispatchCompute(width / workgroup_x, height / workgroup_y, 1); GL_CHECK_ERRORS;
	glMemoryBarrier   ( GL_SHADER_STORAGE_BARRIER_BIT );
	updateBuffers();
}

void WaterComputeShaderProgram::updateBuffers()
{
	GLuint T = SSBO_A;
	SSBO_A = SSBO_B;
	SSBO_B = SSBO_C;
	SSBO_C = T;

	bindBufferBases();
}

void WaterComputeShaderProgram::bindBufferBases()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_A);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO_A);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_B);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO_B);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_C);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, SSBO_C);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void WaterComputeShaderProgram::bindPositions() const
{
	glBindBuffer(GL_ARRAY_BUFFER, SSBO_V);
}

void WaterComputeShaderProgram::bindNormals() const
{
	glBindBuffer(GL_ARRAY_BUFFER, SSBO_N);
}

void WaterComputeShaderProgram::bindTextureCoords() const
{
	glBindBuffer(GL_ARRAY_BUFFER, SSBO_T);
}

WaterComputeShaderProgram::~WaterComputeShaderProgram()
{
	//glDeleteBuffers(1, &SSBO_V);
	//glDeleteBuffers(1, &SSBO_C);
	//glDeleteBuffers(1, &SSBO_B);
	//glDeleteBuffers(1, &SSBO_A);
}