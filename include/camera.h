#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Camera {
public:
    Camera();
    Camera(glm::vec3, glm::vec3, float, float, float, float);     // 利用位置坐标  目标位置坐标 俯角 偏航角 鼠标灵敏度 移动速度初始化一个摄像机类
    void CameraMove(GLFWwindow*);               // 摄像机移动
    glm::mat4 getViewMat();     // 获取当前摄像机的观察矩阵
    
private:
    float pitch;                // 俯角
    float yaw;                  // 偏航角
    float sensitivity;          // 摄像机的鼠标灵敏度
    float cameraSpeed;          // 摄像机的移动速度 先定义四个方向均相同

    glm::vec3 cameraPos;        // 摄像机位置 朝向目标 方向向量  真正朝向方向的向量（前轴z） 右轴x 上轴y
    glm::vec3 cameraTar;
    glm::vec3 cameraDirec;  
    glm::vec3 cameraFront;      // 代表真正朝向的方向      Front = Tar-Pos = -Direc  可根据俯角与偏航角得到
    glm::vec3 cameraRight;      
    glm::vec3 cameraUp; 

    glm::mat4 viewMat;          // 每一个摄像机都有自己的观察矩阵
};

#endif