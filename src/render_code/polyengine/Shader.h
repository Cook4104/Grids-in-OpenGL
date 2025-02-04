#pragma once
#include <glm/mat4x4.hpp>
namespace PolyEngine{
  enum ShaderTypes{
    FRAGMENT = 0x8B30,
    VERTEX = 0x8B31,
  };
  class ShaderProgram{
    private:
      unsigned int id;
    public:
      ~ShaderProgram();
      ShaderProgram();
      void Activate();
      void AddShader(const char* path,ShaderTypes type);
      void CompileShaders();
      int GetUniformLocation(const char* name);
      void Uniform1f(const char* uniformName,float value);      
      void Uniform2f(const char* uniformName,float* value);      
      void Uniform3f(const char* uniformName,float* value);
      void Uniform4f(const char* uniformName,float* value);

      void Uniform1i(const char* uniformName,int value);      
      void Uniform2i(const char* uniformName,int* value);      
      void Uniform3i(const char* uniformName,int* value);
      void Uniform4i(const char* uniformName,int* value);
      void UniformMatrix4f(const char* uniformName,glm::mat4 matrix);
  };
}
