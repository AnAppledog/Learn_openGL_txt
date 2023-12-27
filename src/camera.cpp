#include "camera.h"

Camera::Camera() {
    this->pitch = 0.0f;             // 俯角
    this->yaw = -90.0f;             // 偏航角
    this->fov = 45.0f;              // 视角
    this->sensitivity = 0.025f;     // 镜头灵敏度
    this->cameraSpeed = 2.5f;       // 移动速度

    this->cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);      // 默认位置坐标为（0， 0， 3）  
    this->cameraTar = glm::vec3(0.0f, 0.0f, 0.0f);      // 朝向(0, 0, 0)的方向      根据初始化的俯角与偏航角所得
    this->cameraDirec = glm::normalize(this->cameraPos - this->cameraTar);      // 方向向量（与实际朝向方向相反）
    this->cameraFront = glm::normalize(this->cameraTar - this->cameraPos);      // 真正的视野方向   (0.0f, 0.0f, -1.0f) 刚好与默认的俯角与偏航角对应
    this->cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->cameraDirec));      // 右轴
    this->cameraUp = glm::normalize(glm::cross(this->cameraDirec, this->cameraRight));                  // 上轴

    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, this->cameraUp);       // 创建观察矩阵
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraTar, float pitch, float yaw, float sensitivity, float cameraSpeed) {
    this->pitch = pitch;                // 俯角
    this->yaw = yaw;                    // 偏航角
    this->fov = 45.0f;                  // 视角
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

    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, this->cameraUp);       // 创建观察矩阵
}

void Camera::CameraMove(GLFWwindow* mywindow, float deltaTime) {        // 当前摄像机自由移动 视角不动 即Front不动  所以Pos与Tar应该相对静止 即保持一样的运动
    float true_cameraSpeed = this->cameraSpeed * deltaTime;             // 基于每一帧的间隔所得出的实际速度

    if (glfwGetKey(mywindow, GLFW_KEY_W) == GLFW_PRESS) {               // w朝向视野方向前进
        this->cameraPos += true_cameraSpeed * this->cameraFront;
        this->cameraTar += true_cameraSpeed * this->cameraFront;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_S) == GLFW_PRESS) {               // s朝向视野方向后退
        this->cameraPos -= true_cameraSpeed * this->cameraFront;
        this->cameraTar -= true_cameraSpeed * this->cameraFront;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_A) == GLFW_PRESS) {               // a朝向摄像机右轴负方向移动
        this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * true_cameraSpeed;
        this->cameraTar -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * true_cameraSpeed;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_D) == GLFW_PRESS) {               // d朝向摄像机右轴正方向移动
        this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * true_cameraSpeed;
        this->cameraTar += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * true_cameraSpeed;
    }

    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, glm::vec3(0.0f, 1.0f, 0.0f));       // 基于新的位置向量 重新计算观察矩阵
}

void Camera::CameraMoveFPS(GLFWwindow* mywindow, float deltaTime) {        
    float true_cameraSpeed = this->cameraSpeed * deltaTime;             
    glm::vec3 cameraMoveFront = glm::vec3(this->cameraFront.x, 0, this->cameraFront.z);    // 不能随意飞行 即规定y分量为0即可

    if (glfwGetKey(mywindow, GLFW_KEY_W) == GLFW_PRESS) {               
        this->cameraPos += true_cameraSpeed * cameraMoveFront;
        this->cameraTar += true_cameraSpeed * cameraMoveFront;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_S) == GLFW_PRESS) {               
        this->cameraPos -= true_cameraSpeed * cameraMoveFront;
        this->cameraTar -= true_cameraSpeed * cameraMoveFront;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_A) == GLFW_PRESS) {               
        this->cameraPos -= glm::normalize(glm::cross(cameraMoveFront, this->cameraUp)) * true_cameraSpeed;
        this->cameraTar -= glm::normalize(glm::cross(cameraMoveFront, this->cameraUp)) * true_cameraSpeed;
    }
    if (glfwGetKey(mywindow, GLFW_KEY_D) == GLFW_PRESS) {               
        this->cameraPos += glm::normalize(glm::cross(cameraMoveFront, this->cameraUp)) * true_cameraSpeed;
        this->cameraTar += glm::normalize(glm::cross(cameraMoveFront, this->cameraUp)) * true_cameraSpeed;
    }

    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, glm::vec3(0.0f, 1.0f, 0.0f));       // 基于新的位置向量 重新计算观察矩阵
}

void Camera::CameraView(float xoffset, float yoffset) {
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->pitch += yoffset;
    this->yaw += xoffset;

    // 限制一下视野的上下转动范围
    if(this->pitch > 89.0f)
        this->pitch =  89.0f;
    if(this->pitch < -89.0f)
        this->pitch = -89.0f;
    
    updateCamera();
}

void Camera::Camerafov(float offset) {
    if(this->fov >= 1.0f && this->fov <= 45.0f)
        this->fov -= offset;
    if(this->fov <= 1.0f)
        this->fov = 1.0f;
    if(this->fov >= 45.0f)
        this->fov = 45.0f;
}
void Camera::updateCamera() {
    // 先计算新的视野方向
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->cameraFront = glm::normalize(front);

    // 再计算目标坐标 前轴 上轴 右轴
    this->cameraTar = this->cameraPos + this->cameraFront;
    this->cameraDirec = -this->cameraFront;
    this->cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), this->cameraDirec));      
    this->cameraUp = glm::normalize(glm::cross(this->cameraDirec, this->cameraRight));       

    // 最后更新矩阵           
    this->viewMat = glm::lookAt(this->cameraPos, this->cameraTar, glm::vec3(0.0f, 1.0f, 0.0f));       // 基于新的位置向量 重新计算观察矩阵
}

float Camera::getViewfov() {
    return this->fov;
}

glm::mat4 Camera::getViewMat() {
    return this->viewMat;
}