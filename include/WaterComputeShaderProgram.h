#ifndef WATERCOMPUTESHADERPROGRAM_H
#define WATERCOMPUTESHADERPROGRAM_H

#include "common.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include "ShaderProgram.h"

struct Vertex
{
    vec3 position;
    vec3 normal;
    vec2 texCoord;
};

class WaterComputeShaderProgram : public ShaderProgram
{
    GLuint SSBO_A;
    GLuint SSBO_B;
    GLuint SSBO_C;
    GLuint SSBO_V;
    GLuint SSBO_N;
    GLuint SSBO_T;

    int width;
    int height;

    int workgroup_x;
    int workgroup_y;



    public:
        WaterComputeShaderProgram(const std::unordered_map<GLenum, std::string>& shaders,int width, int height, int w_x = 32, int w_y = 32) :
            ShaderProgram(shaders),
            width(width),
            height(height),
            workgroup_x(w_x),
            workgroup_y(w_y)
        {
            createBuffers();
            initializeBuffer();
        }

        ~WaterComputeShaderProgram();
        GLuint returnVertices() const;
        void startComputing();
        void bindPositions() const;
        void bindNormals() const;
        void bindTextureCoords() const;

    private:
    
        void createBuffers ();
        void initializeBuffer();
        void updateBuffers();
        void bindBufferBases();       
};

#endif