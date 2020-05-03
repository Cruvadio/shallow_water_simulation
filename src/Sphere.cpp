
#include "Sphere.h"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
using std::vector;

Sphere::Sphere (const char* path,const ShaderProgram& shaderProgram, float radius,const vec3& center)
{
    vector<glm::vec3> vertices;
    vector<glm::vec2> uvs;
    vector<glm::vec3> normals;
    glGenBuffers(1, &VVBO);
    glGenBuffers(1, &TVBO);
    glGenBuffers(1, &NVBO);
    glGenVertexArrays(1, &VAO);
    
    bool err = loadOBJ(path, vertices, uvs, normals);
    if (!err) throw std::runtime_error("File name error");

    size = vertices.size();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);  GL_CHECK_ERRORS;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, NVBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size()* sizeof(vec3), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);  GL_CHECK_ERRORS;
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, TVBO);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), uvs.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);  GL_CHECK_ERRORS;
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)0);

    glBindVertexArray(0);

    sphereRadius = radius;
    sphereCenter = center;

    shader = shaderProgram;
    model = mat4(1.0);
    mat4 tr = translate(model, center);
    model = scale(model, vec3(radius));
    model = tr * model;
}


void Sphere::drawSphere(const glm::mat4& projection, 
                    const glm::mat4& view, 
                    const glm::vec3& lightPos,
                    const glm::vec3& cameraPos,
                    int width,
                    int height)
{
    shader.StartUseShader();
    shader.SetUniform("projection", projection);
    shader.SetUniform("view", view);
    shader.SetUniform("model", model);
    shader.SetUniform("lightPos", lightPos);
    shader.SetUniform("sphereRadius", sphereRadius);
    shader.SetUniform("width", width);
    shader.SetUniform("height", height);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, size);
    glBindVertexArray(0);

    shader.StopUseShader();
}