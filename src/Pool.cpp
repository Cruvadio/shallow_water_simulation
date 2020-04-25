#include "Pool.h"

Pool::Pool(const glm::mat4 mod,const ShaderProgram& shader, const std::string& path)
{
    texture = loadTexture(path.c_str());
    model = mod;
    this->shader = shader;
    this->shader.StartUseShader();
    this->shader.SetUniform("wallTexture", 0);
    this->shader.StopUseShader();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * 3 * sizeof(float)));
}

void Pool::drawPool(const glm::mat4& projection, 
                    const glm::mat4& view, 
                    const glm::vec3& lightPos,
                    const glm::vec3& cameraPos,
                    const glm::vec3& lightColor
                   )
{
    glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    glEnable(GL_DEPTH_TEST);
    shader.StartUseShader();
    shader.SetUniform("model", model);
    shader.SetUniform("projection", projection);
    shader.SetUniform("view", view);
    shader.SetUniform("lightPos", lightPos);
    shader.SetUniform("cameraPos", cameraPos);
    shader.SetUniform("lightColor", lightColor);

    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glDrawArrays(GL_TRIANGLES, 0, 30);

    shader.StopUseShader();
    glDisable(GL_CULL_FACE);
}


