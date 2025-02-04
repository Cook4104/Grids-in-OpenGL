#pragma once

namespace PolyEngine{
  class Image{
  public:
    unsigned char* image_data;
    int colorChannels = 0,width = 0,height = 0;
    Image(const char* path);
    ~Image();
  };
  
  class Texture{
  private:
    unsigned int id;
  public:
    Texture(Image& image);
    void TexParameterI(int parameter,int param);
    void TexParameterF(int parameter,float param);
    void GenerateMipMaps();
    void Bind();
    void UseTexture(int slot);
    ~Texture();
  };
}
