#include "Textures.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>

using namespace PolyEngine;

GLint GetInternalFormat(int colorChannel){
  switch(colorChannel){
    case 4:
      return GL_RGBA;
    case 3:
      return GL_RGB;
    case 2:
      return GL_RG;
    case 1:
      return GL_RED;
    default:
      return GL_RGBA;
  }
}

Image::Image(const char* path){
  stbi_set_flip_vertically_on_load(1);
  image_data = stbi_load(path, &width, &height, &colorChannels,0);
}
Image::~Image(){
  stbi_image_free(image_data);
}


Texture::Texture(Image& image){
  glGenTextures(1,&id);
  glBindTexture(GL_TEXTURE_2D,id);
  glTexImage2D(GL_TEXTURE_2D, 0,GetInternalFormat(image.colorChannels),image.width,image.height,0,GetInternalFormat(image.colorChannels),GL_UNSIGNED_BYTE,image.image_data);
}
void Texture::GenerateMipMaps() {
    glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture::TexParameterI(int parameter,int param){
  glTextureParameteri(id,parameter,param);
  /*Bind();*/
  /*glTexParameteri(GL_TEXTURE_2D, parameter, param);*/
}
void Texture::TexParameterF(int parameter,float param){
  glTextureParameterf(id,parameter,param);
}
void Texture::Bind(){
  glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::UseTexture(int slot){
  Bind();
  glActiveTexture(GL_TEXTURE0 + slot);
}
Texture::~Texture(){
  glDeleteTextures(1, &id);
}
