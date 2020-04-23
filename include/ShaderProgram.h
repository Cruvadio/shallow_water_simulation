#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <unordered_map>
#include "common.h"
#include <glm/glm.hpp>

using namespace glm;

class ShaderProgram
{
public:

  ShaderProgram() : shaderProgram(-1) {};

  ShaderProgram(const std::unordered_map<GLenum, std::string> &inputShaders);

  virtual ~ShaderProgram() {};

  void Release(); //actual destructor

  virtual void StartUseShader() const;

  virtual void StopUseShader() const;

  GLuint GetProgram() const { return shaderProgram; }


  bool reLink();

  void SetTexture(const std::string &location, int value) const;

  void SetUniform(const std::string &location, float value) const;

  void SetUniform(const std::string &location, double value) const;

  void SetUniform(const std::string &location, int value) const;

  void SetUniform(const std::string &location, unsigned int value) const;

  void SetUniform(const std::string &location, const vec2& value) const;

  void SetUniform(const std::string &location, const vec3& value) const;

  void SetUniform(const std::string &location, const vec4& value) const;

  void SetUniform(const std::string &location, const mat4& value) const;

  void SetUniform(const std::string &location, const mat3& value) const;

  void SetUniform(const std::string &location, const uvec2& value) const;

  void SetUniform(const std::string &location, const uvec3& value) const;

  void SetUniform(const std::string &location, const uvec4& value) const;

  void SetUniform(const std::string &location, const ivec2& value) const;

  void SetUniform(const std::string &location, const ivec3& value) const;

  void SetUniform(const std::string &location, const ivec4& value) const;
private:
  static GLuint LoadShaderObject(GLenum type, const std::string &filename);

  GLuint shaderProgram;
  std::unordered_map<GLenum, GLuint> shaderObjects;
};


#endif