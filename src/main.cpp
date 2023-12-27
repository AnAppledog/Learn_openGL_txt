#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "gl_init.h"
#include "shader.h"
#include "stb_image.h"
#include "camera.h"

#define FPS 0               // 通过宏定义修改摄像机模式 1代表FPS模式 0代表自由模式

// 创建一个摄像机对象 摄像机位置与朝向为默认值
Camera myCamera;

float mixValue = 0.5f;      // 两个贴图的混合参数 初始化为0.5
float deltaTime = 0.0f;     // 用来计算每一帧之间的间隔时间
float lastTime = 0.0f;

float lastX = 400, lastY = 300; // 用来计算每一帧之间鼠标的移动距离
bool firstMouse = true;     // 判断光标是否第一次进入窗口

// 定义摄像机的位置向量 方向向量 上向量
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float vertices_[] = {
    // 位置                // 贴图坐标
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] = {       // 十个正方体的位置信息
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};

void processInput(GLFWwindow* window);
void frambuffer_size_callback(GLFWwindow*, int, int);
void mouse_callback(GLFWwindow*, double, double);     // 鼠标控制回调函数
void scroll_callback(GLFWwindow*, double, double);      // 鼠标滚轮回调函数

int main() {
    GLFWwindow* mywindow;
    if ((mywindow = glfw_init()) == NULL) { // 初始化  若失败则输出错误信息并返回
        std::cout << "Failed to init a window" << std::endl;
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);         // 打开一个 （0，800）（0，600）的视口 即可绘制部分
    
    glfwSetFramebufferSizeCallback(mywindow, frambuffer_size_callback);     // 给窗口注册回调函数
    glfwSetCursorPosCallback(mywindow, mouse_callback);                     // 鼠标回调函数
    glfwSetScrollCallback(mywindow, scroll_callback);                         // 鼠标滚轮回调函数

    // 将顶点属性信息存储在VAO中
    unsigned int VAO;   
    glGenVertexArrays(1, &VAO);         // VAO的初始化定义
    glBindVertexArray(VAO);             // VAO的绑定

    // 将顶点信息存储在显卡的内存中，利用VBO顶点缓冲对象管理
    unsigned int VBO;
    glGenBuffers(1, &VBO);              // 生成一个缓冲对象ID-VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // 将VBO与顶点缓冲对象绑定
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_STATIC_DRAW);  // 将顶点信息传入VBO缓冲内存中

    // 将顶点索引信息利用EBO元素缓冲对象管理
    // unsigned int EBO;
    // glGenBuffers(1, &EBO);              // 生成一个缓冲对象ID-EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // 将EBO与元素缓冲对象绑定
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // 将索引信息传入EBO缓冲内存中

    // 配置0号位置的顶点属性信息  会存储在已经绑定的VAO中  0号位置用来存储顶点的位置信息
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);   // 启用0号顶点属性
    // 配置1号位置的顶点属性信息   用来存储顶点的纹理坐标信息
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);   // 启用1号顶点属性

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
    unsigned char* data1 = stbi_load("textures/expression.png", &widths1, &heights1, &nrChannels1, 0);
    if (data1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widths1, heights1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to laod jpg" << std::endl;
    }
    stbi_image_free(data1);      // 释放存放图片数据的变量

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // 开启线框模式
    glEnable(GL_DEPTH_TEST);    // 启用深度测试
    glfwSetInputMode(mywindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);   // 隐藏光标

    // 创建一个着色器程序对象  利用已经编写好的着色器代码
    Shader myShader("shaderSource/vs.txt", "shaderSource/fs.txt");
    myShader.use();                         // 启用着色器程序对象

    myShader.setInt("myTexture0", 0);       // 为对应的采样器设置对应的纹理单元
    myShader.setInt("myTexture1", 1);

    // 不断绘制图像并接受输入
    while(!glfwWindowShouldClose(mywindow)) {   // 判断是否关闭 不关闭即无限循环 每一次循环为一帧
        processInput(mywindow);                                     // 处理键盘输入

        glClearColor(0.2f, 0.3f, 0.2f, 1.0f);                   // 设置清屏颜色状态
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // 用设置好的颜色状态进行清屏操作 并开启Z缓冲

        float timeValue = glfwGetTime();
        float timeC = (cos(timeValue) * 0.5) + 0.5f;
        myShader.setFloat("offset_c", timeC);   // 设置颜色的偏移量
        myShader.setFloat("mixV", mixValue);    // 设置贴图混合参数

        
        glm::mat4 viewMat;                      // 创建一个观察矩阵
        glm::mat4 projMat;                      // 创建一个透视矩阵 
                                                // 朝向随俯角与偏航角变化
        viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            
        projMat = glm::perspective(glm::radians(myCamera.getViewfov()), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);    // 透视矩阵
        
        // myShader.setMat4("view", myCamera.getViewMat());
        myShader.setMat4("view", myCamera.getViewMat());
        myShader.setMat4("projection", projMat);


        glBindVertexArray(VAO);                 // 使用VAO存储的顶点属性信息
        for (int i = 0; i < 10; i++) {          // 十个立方体 模型矩阵不同 在世界中的位置也就不同
            glm::mat4 modelMat;                 // 创建一个模型矩阵
            modelMat = glm::translate(modelMat, cubePositions[i]);
            
            modelMat = glm::rotate(modelMat, timeValue * glm::radians(20.0f * (i + 1)), glm::vec3(0.5f, 1.0f, 0.0f)); // 让立方体旋转

            myShader.setMat4("model", modelMat);

            glDrawArrays(GL_TRIANGLES, 0, 36);      // 36个三角形 组成一个正方体
        }
        

        glfwSwapBuffers(mywindow);              // 判断是否有事件触发（鼠标 键盘等）并负责更新窗口状态 调用相应的回调函数
        glfwPollEvents();                       // 交换颜色缓冲区 输出显示
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // glDeleteBuffers(1, &EBO);

    glfwTerminate();        // 关闭GLFW
    return 0;
}

// 处理键盘输入的函数
void processInput(GLFWwindow* mywindow) {
    float currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;     // 计算这一帧与上一帧的间隔时间
    lastTime = currentTime;

    if (glfwGetKey(mywindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(mywindow, true);
    }
    if (glfwGetKey(mywindow, GLFW_KEY_UP) == GLFW_PRESS) {
        mixValue += 0.003f;
        if (mixValue > 1.0f)
            mixValue = 1.0f;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mixValue -= 0.003f;
        if (mixValue < 0.0f)
            mixValue = 0.0f;
    }
    
    if (FPS == 1) 
        myCamera.CameraMoveFPS(mywindow, deltaTime);                  // 处理摄像机移动信息
    else    
        myCamera.CameraMove(mywindow, deltaTime);

    return;
}

// 处理鼠标输入的回调函数
void mouse_callback(GLFWwindow* mywindow, double xPos, double yPos) {
    if (mywindow == NULL) {
        std::cout << "This window is false" << std::endl;
    }

    if(firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float offset_mouse_x = xPos - lastX;
    float offset_mouse_y = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    myCamera.CameraView(offset_mouse_x, offset_mouse_y);

    return;
}

// 视角随滚轮缩放的回调函数
void scroll_callback(GLFWwindow* mywindow, double xoffset, double yoffset) {
    if (mywindow == NULL) {                     // 防止编译警告
        std::cout << "This window is false" << std::endl;
        xoffset++;
    }
    myCamera.Camerafov(yoffset);
    
    return;
}

// 视口随窗口大小变化而变化的回调函数
void frambuffer_size_callback(GLFWwindow* mywindow, int wid, int hei) {
    if (mywindow == NULL) {
        std::cout << "This window is false" << std::endl;
    }
    glViewport(0, 0, wid, hei);
}   