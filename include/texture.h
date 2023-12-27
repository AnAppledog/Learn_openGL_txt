#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 创建一个2D纹理对象
class Texture2D {
public:
    Texture2D();       // 利用传入的ID初始化
    void Generate(const char*);        // 加载纹理数据
    void Bind();            // 绑定
    GLint GetID();          // 获取ID
private:
    GLuint ID;
    GLuint Width, Height;   // 纹理高度与宽度
    
    GLuint Internal_Format; // 将纹理存储为何种格式
    GLuint Image_Format;    // 原图的格式
    
    GLuint Wrap_S;          // S轴的环绕方式
    GLuint Wrap_T;          // T轴的环绕方式
    GLuint Filter_Min;      // 缩小时的过滤方式
    GLuint Filter_Max;      // 放大时的过滤方式
};

#endif