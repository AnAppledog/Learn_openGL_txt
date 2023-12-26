#include "camera.h"

Camera::Camera() {
    this->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      // 默认位置坐标为（0， 0， 3）
    this->cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   // 朝向(0, 0, 1)的方向
    this->cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       // 构建观察矩阵
    this->viewMat = glm::lookAt(this->cameraPos, this->cameraFront, this->cameraUp);
}

glm::mat4 Camera::getViewMat() {
    return this->viewMat;
}