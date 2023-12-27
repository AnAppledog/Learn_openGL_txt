#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture2D::Texture2D()          // 默认格式均为RGB          默认环绕方式均为重复    默认过滤方式均为线性过滤
    : Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->ID);
}

void Texture2D::Generate(const char* path) {
    stbi_set_flip_vertically_on_load(true);     // 颠倒y轴

    int wid, hei, nrChannels;
    unsigned char* data = stbi_load(path, &wid, &hei, &nrChannels, 0);      // 从指定路径读取贴图文件

    glBindTexture(GL_TEXTURE_2D, this->ID);     // 绑定

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);    // 根据纹理对象的属性设置该单元中环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, Internal_Format, wid, hei, 0, Image_Format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        this->Width = wid;
        this->Height = hei;
    }
    else {
        std::cout << "Failed to laod jpg" << std::endl;
    }
    stbi_image_free(data);              // 释放存放图片数据的变量

    glBindTexture(GL_TEXTURE_2D, 0);    // 解绑定
}

void Texture2D::Bind() {
    glBindTexture(GL_TEXTURE_2D, this->ID);     // 绑定此纹理对象
}

GLint Texture2D::GetID() {
    return this->ID;
}