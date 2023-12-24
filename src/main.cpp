#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "gl_init.h"
#include "shader.h"
#include "stb_image.h"
// 调整窗口大小
//void frambuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//    return;
//}

float vertices[] {          // 用flaot数组定义顶点的标准化设备坐标 每一行分别对应一个顶点的x、y、z轴坐标
    //位置                  颜色        每个顶点都有相对应的独特的颜色      纹理坐标
    -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,         0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, 1.0f,         1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f, 1.0f,         1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, 1.0f,         0.0f, 1.0f
    
};

unsigned int indices[] {    // 顶点索引数组
    0, 1, 2,                // 第一个三角形
    0, 2, 3                 // 第二个三角形
};

void processInput(GLFWwindow* window);

int main() {
    GLFWwindow* mywindow;
    if ((mywindow = glfw_init()) == NULL) { // 初始化  若失败则输出错误信息并返回
        std::cout << "Failed to init a window" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);         // 打开一个 （0，800）（0，600）的视口 即可绘制部分
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 开启线框模式

    // 将顶点属性信息存储在VAO中
    unsigned int VAO;   
    glGenVertexArrays(1, &VAO);         // VAO的初始化定义
    glBindVertexArray(VAO);             // VAO的绑定

    // 将顶点信息存储在显卡的内存中，利用VBO顶点缓冲对象管理
    unsigned int VBO;
    glGenBuffers(1, &VBO);              // 生成一个缓冲对象ID-VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 将VBO与顶点缓冲对象绑定
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // 将顶点信息传入VBO缓冲内存中

    // 将顶点索引信息利用EBO元素缓冲对象管理
    unsigned int EBO;
    glGenBuffers(1, &EBO);              // 生成一个缓冲对象ID-EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 将EBO与元素缓冲对象绑定
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // 将索引信息传入EBO缓冲内存中

    // 配置0号位置的顶点属性信息  会存储在已经绑定的VAO中  0号位置用来存储顶点的位置信息
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);   // 启用0号顶点属性
    //配置1号位置的顶点属性信息   用来存储顶点的渲染颜色信息
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   // 启用1号顶点属性
    //配置2号位置的顶点属性信息   用来存储顶点的纹理坐标信息
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);   // 启用1号顶点属性

    // 创建两个个纹理对象ID
    unsigned int texture[2];
    glGenTextures(2, texture);      // 先生成一个对象ID

    glActiveTexture(GL_TEXTURE0);                   // 激活纹理单元0
    glBindTexture(GL_TEXTURE_2D, texture[0]);       // 将第一个ID与2D纹理对象绑定 并存放在纹理单元0内

    // 设置该单元中环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // 将s、t轴均设置为重复环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 均设置为线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);     // 颠倒y轴
    
    // 传入贴图数据
    int widths, heights, nrChannels;
    unsigned char* data = stbi_load("textures/sister.jpeg", &widths, &heights, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widths, heights, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to laod jpg" << std::endl;
    }
    stbi_image_free(data);      // 释放存放图片数据的变量



    glActiveTexture(GL_TEXTURE1);                   // 激活纹理单元1
    glBindTexture(GL_TEXTURE_2D, texture[1]);       // 将第2个ID与2D纹理对象绑定 并存放在纹理单元1内

    // 设置该单元中环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   // 将s、t轴均设置为重复环绕方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   // 均设置为线性过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // 传入贴图数据
    int widths1, heights1, nrChannels1;
    unsigned char* data1 = stbi_load("textures/sister.jpeg", &widths1, &heights1, &nrChannels1, 0);
    if (data1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widths1, heights1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to laod jpg" << std::endl;
    }
    stbi_image_free(data1);      // 释放存放图片数据的变量


    // 创建一个着色器程序对象  利用已经编写好的着色器代码
    Shader myshader("shaderSource/vs.txt", "shaderSource/fs.txt");
    myshader.use();                         // 启用着色器程序对象

    myshader.setInt("myTexture0", 0);       // 为对应的采样器设置对应的纹理单元
    myshader.setInt("myTexture1", 1);

    // 不断绘制图像并接受输入
    while(!glfwWindowShouldClose(mywindow)) {   // 判断是否关闭 不关闭即无限循环
        processInput(mywindow);                 //处理键盘输入

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // 设置清屏颜色状态
        glClear(GL_COLOR_BUFFER_BIT);           // 用设置好的颜色状态进行清屏操作

        float timeValue = glfwGetTime();
        float timeX = sin(timeValue) * 0.5;
        float timeY = cos(timeValue) * 0.5;
        float timeC = (cos(timeValue) * 0.5) + 0.5f;
        myshader.setFloat("offset_x", timeX);   // 设置x轴偏移量
        myshader.setFloat("offset_y", timeY);   // 设置y轴偏移量
        myshader.setFloat("offset_c", timeC);   // 设置颜色的偏移量

        glActiveTexture(GL_TEXTURE0);           // 对于不同的纹理单元使用相对应的纹理对象ID
        glBindTexture(GL_TEXTURE_2D, texture[0]);   
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        
        glBindVertexArray(VAO);                 // 使用VAO存储的顶点属性信息
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);   // 从索引的第0个开始 每三个顶点画一个三角形

        glfwSwapBuffers(mywindow);              // 判断是否有事件触发（鼠标 键盘等）并负责更新窗口状态 调用相应的回调函数
        glfwPollEvents();                       // 交换颜色缓冲区 输出显示
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();        // 关闭GLFW
    return 0;
}

// 处理键盘输入的函数
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    return;
}