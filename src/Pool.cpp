#include "Pool.h"

Pool::Pool(const glm::mat4 mod,
           const ShaderProgram& shader,
           const std::string& base,
           const std::string& normals)
{
    this->base = loadTexture(base.c_str());
    this->normal = loadTexture(normals.c_str());
    model = mod;
    this->shader = shader;
    this->shader.StartUseShader();
    this->shader.SetUniform("wallTexture", 0);
    this->shader.SetUniform("normalMap", 1);
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
                    const glm::vec3& sphereCenter,
                    float sphereRadius,
                    int width,
                    int height
                   )
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    shader.StartUseShader(); GL_CHECK_ERRORS;
    shader.SetUniform("model", model); GL_CHECK_ERRORS;
    shader.SetUniform("projection", projection); GL_CHECK_ERRORS;
    shader.SetUniform("view", view); GL_CHECK_ERRORS;
    shader.SetUniform("lightPos", lightPos); GL_CHECK_ERRORS;
    shader.SetUniform("sphereCenter", sphereCenter); GL_CHECK_ERRORS;
    shader.SetUniform("sphereRadius", sphereRadius); GL_CHECK_ERRORS;
    shader.SetUniform("width", width); GL_CHECK_ERRORS;
    shader.SetUniform("height", height); GL_CHECK_ERRORS;

    glBindVertexArray(VAO); GL_CHECK_ERRORS;
    glActiveTexture(GL_TEXTURE0); GL_CHECK_ERRORS;
    glBindTexture(GL_TEXTURE_2D, base); GL_CHECK_ERRORS;

    glActiveTexture(GL_TEXTURE1); GL_CHECK_ERRORS;
    glBindTexture(GL_TEXTURE_2D, normal); GL_CHECK_ERRORS;



    glDrawArrays(GL_TRIANGLES, 0, 30); GL_CHECK_ERRORS;

    shader.StopUseShader(); GL_CHECK_ERRORS;
    glDisable(GL_CULL_FACE);
}


