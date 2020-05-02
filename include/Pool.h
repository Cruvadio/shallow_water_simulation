#ifndef POOL_H
#define POOL_H

#include "common.h"
#include "ShaderProgram.h"
#include "glm/glm.hpp"

static float vertices[] = {
    // positions          // normals           // texture coords
    //back
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    //front    
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, 
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    
    //left
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,  
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,  
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,

    //right
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    //bottom
     1.0f, -1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  1.0f, 1.0f,
     -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
     -1.0f, -1.0f, -1.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f, 1.0f,  0.0f,  1.0f, 0.0f,
    /*
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f */
};


class Pool
{
    int base;
    int normal;
    int height;
    glm::mat4 model;
    ShaderProgram shader;
    
    GLuint VAO, VBO;

    public:
        Pool(const glm::mat4 mod,
           const ShaderProgram& shader,
           const std::string& base,
           const std::string& normals);

        void drawPool(const glm::mat4& projection, 
                    const glm::mat4& view, 
                    const glm::vec3& lightPos,
                    const glm::vec3& cameraPos,
                    const glm::vec3& sphereCenter,
                    float sphereRadius,
                    int width,
                    int height
                   );

        void bindTexture(GLenum t)
        {
            glActiveTexture(t);
            glBindTexture(GL_TEXTURE_2D, base);

            glActiveTexture(t + 1);
            glBindTexture(GL_TEXTURE_2D, normal);

            //glActiveTexture(t + 2);
            //glBindTexture(GL_TEXTURE_2D, height);
        }

        void cleanUp()
        {
            glDeleteBuffers(1, &VBO);
            glDeleteVertexArrays(1, &VAO);
        }

};

#endif