#include "Skybox.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

unsigned int Skybox::loadCubeMap (const std::vector<std::string> textures)
        {
            unsigned int textureID;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

            int width, height, nrChannels;
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                unsigned char *data = stbi_load(textures[i].c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }
                else
                {
                    std::cout << "Cubemap texture failed to load at path: " << textures[i] << std::endl;
                    stbi_image_free(data);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            return textureID;
        }

void Skybox::drawSkybox(const mat4& projection, const mat4& view)
{
        glDepthFunc(GL_LEQUAL); 
        shader.StartUseShader();
        this->view = glm::mat4(glm::mat3(view));
        this->projection = projection; 
        shader.SetUniform("view", this->view);
        shader.SetUniform("projection", projection);
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        shader.StopUseShader();
        glDepthFunc(GL_LESS); 
}