#include "camera.h"

Camera::Camera() {
    this->pitch = 0.0f;             // 俯角
    this->yaw = -90.0f;             // 偏航角
    this->sensitivity = 0.0025f;    // 镜头灵敏度
    this->cameraSpeed = 0.01f;      // 移动速度

    this->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      // 默认位置坐标为（0， 0， 3）  
    this->cameraTar = glm::vec3(0.0f, 0.0f, 0.0f);      // 朝向(0, 0, 0)的方向      根据初始化的俯角与偏航角所得
    this->cameraDirec = glm::normalize(this->cameraPos - this->cameraTar);      // 方向向量（与实际朝向方向相反）
    this->cameraFront = glm::normalize(this->cameraTar - this->cameraPos);      // 真正的视野方向   (0.0f, 0.0f, -1.0f) 刚好与默认的俯角与偏航角对应
    this->cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->cameraDirec));      // 右轴
    this->cameraUp = glm::normalize(glm::cross(this->cameraDirec, this->cameraRight));                  // 上轴

    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, glm::vec3(0.0f, 1.0f, 0.0f));       // 创建观察矩阵
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraTar, float pitch, float yaw, float sensitivity, float cameraSpeed) {
    this->pitch = pitch;                // 俯角
    this->yaw = yaw;                    // 偏航角
    this->sensitivity = sensitivity;    // 镜头灵敏度
    this->cameraSpeed = cameraSpeed;    // 移动速度

    this->cameraPos = cameraPos;        // 位置坐标                                  指向的坐标位置 由俯角与偏转角的到
    this->cameraTar = cameraTar;        // 目标坐标
    this->cameraDirec = glm::normalize(this->cameraPos - this->cameraTar);      // 方向向量（与实际朝向方向相反）
    this->cameraFront = glm::normalize(this->cameraTar - this->cameraPos);      // 真正的视野方向   
    if (this->cameraFront != glm::normalize(glm::vec3(cos(glm::radians(pitch)) * cos(glm::radians(yaw)), sin(glm::radians(pitch)), cos(glm::radians(pitch)) * sin(glm::radians(yaw))))) {
        // 由目标坐标与摄像机坐标计算的到的视野方向应该与欧拉角对应的方向相同
        // 不相同则说明给出的初始化数据有错 （相互矛盾）
        std::cout << "Failed to init a camera" << std::endl;
        std::cout << "Invalid data" << std::endl;
    }
    this->cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->cameraDirec));      // 右轴
    this->cameraUp = glm::normalize(glm::cross(this->cameraDirec, this->cameraRight));                  // 上轴

    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, glm::vec3(0.0f, 1.0f, 0.0f));       // 创建观察矩阵
}

void Camera::CameraMove(GLFWwindow* mywindow) {         // 当前摄像机移动 视角不动 即Front不动  所以Pos与Tar应该相对静止 即保持一样的运动
    if (glfwGetKey(mywindow, GLFW_KEY_W) == GLFW_PRESS) {   
        this->cameraPos += this->cameraSpeed * this->cameraFront;
        this->cameraTar += this->cameraSpeed * this->cameraFront;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_S) == GLFW_PRESS) {
        this->cameraPos -= this->cameraSpeed * this->cameraFront;
        this->cameraTar -= this->cameraSpeed * this->cameraFront;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_A) == GLFW_PRESS) {
        this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * this->cameraSpeed;
        this->cameraTar -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * this->cameraSpeed;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_D) == GLFW_PRESS) {
        this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * this->cameraSpeed;
        this->cameraTar += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * this->cameraSpeed;
    }
}
glm::mat4 Camera::getViewMat() {
    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, glm::vec3(0.0f, 1.0f, 0.0f));       // 创建观察矩阵并返回

    return this->viewMat;
}