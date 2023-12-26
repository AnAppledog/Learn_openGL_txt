#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera();
    glm::mat4 getViewMat();     // 获取当前摄像机的观察矩阵
      
private:
    glm::vec3 cameraPos;        // 摄像机位置 朝向 上向量
    glm::vec3 cameraFront;
    glm::vec3 cameraUp; 

    glm::mat4 viewMat;          // 每一个摄像机都有自己的观察矩阵
};

#endif