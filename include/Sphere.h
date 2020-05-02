#ifndef SPHERE_H
#define SPHERE_H

#include "common.h"
#include "ShaderProgram.h"

class Sphere
{
    GLuint VAO, VVBO, TVBO, NVBO;

    ShaderProgram shader;
    int size;

    float sphereRadius;
    vec3 sphereCenter;

    glm::mat4 model;

    public:
        Sphere (const char* path,const ShaderProgram& shaderProgram, float radius,const vec3& center);

        void drawSphere(const glm::mat4& projection, 
                    const glm::mat4& view, 
                    const glm::vec3& lightPos,
                    const glm::vec3& cameraPos,
                    int width,
                    int height);

        void cleanUp ()
        {
            glDeleteBuffers(1, &VVBO);
            glDeleteBuffers(1, &NVBO);
            glDeleteBuffers(1, &TVBO);
            glDeleteVertexArrays(1, &VAO);
        }

};

#endif