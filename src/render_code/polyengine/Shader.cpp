#include "Shader.h"
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

using namespace PolyEngine;

static std::vector<GLuint> shaders;

static void ClearShaders(){
  for(size_t i = 0;i < shaders.size();i++){
    glDeleteShader(shaders.size());
  }
  shaders.clear();
}

ShaderProgram::ShaderProgram(){
  id = glCreateProgram();
}
ShaderProgram::~ShaderProgram(){
  glDeleteProgram(id);
}

std::string GetShaderContents(const char* path){
  std::ifstream shaderFile(path);
  std::string outputString = "";
  std::string temp = "";
  while(std::getline(shaderFile,temp)){
    outputString += temp+"\n"; 
  }
  return outputString;
}

void ShaderProgram::Activate(){
  glUseProgram(id);
}

void ShaderProgram::AddShader(const char* path,ShaderTypes type){
  GLuint shaderID = glCreateShader(type);
  std::string sources = GetShaderContents(path);
  const char* sources_cstr = sources.c_str();
  glShaderSource(shaderID, 1,&sources_cstr,NULL);
  shaders.push_back(shaderID);
}
void ShaderProgram::CompileShaders(){
  char logs[512] = "";
  int success;
  for(GLuint shader : shaders){
    glCompileShader(shader);
    glAttachShader(id, shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
      glGetShaderInfoLog(shader,512, NULL, logs);
    }
  } 
  glLinkProgram(id);
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(id, 512, NULL, logs);
  }
  ClearShaders();
}
int ShaderProgram::GetUniformLocation(const char* name){
  return glGetUniformLocation(id, name);
}
void ShaderProgram::Uniform1f(const char* uniformName,float value){
  glUniform1f(GetUniformLocation(uniformName),value); 
}
void ShaderProgram::Uniform2f(const char* uniformName,float* value){
  glUniform2f(GetUniformLocation(uniformName),value[0],value[1]); 
}
void ShaderProgram::Uniform3f(const char* uniformName,float* value){
  glUniform3f(GetUniformLocation(uniformName),value[0],value[1],value[2]); 
}
void ShaderProgram::Uniform4f(const char* uniformName,float* value){
  glUniform4f(GetUniformLocation(uniformName),value[0],value[1],value[2],value[3]); 
}


void ShaderProgram::Uniform1i(const char* uniformName,int value){
  glUniform1i(GetUniformLocation(uniformName),value);
}      
void ShaderProgram::Uniform2i(const char* uniformName,int* value){
  glUniform2i(GetUniformLocation(uniformName),value[0],value[1]); 
}      
void ShaderProgram::Uniform3i(const char* uniformName,int* value){
  glUniform3i(GetUniformLocation(uniformName),value[0],value[1],value[2]); 
}
void ShaderProgram::Uniform4i(const char* uniformName,int* value){
  glUniform4i(GetUniformLocation(uniformName),value[0],value[1],value[2],value[3]); 
}

void ShaderProgram::UniformMatrix4f(const char* uniformName,glm::mat4 matrix){
  glUniformMatrix4fv(GetUniformLocation(uniformName),1,GL_FALSE,glm::value_ptr(matrix));
}
