#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/* 一些默认值 */
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // 摄像机位置
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    // 世界y轴
    glm::vec3 WorldUp;

    // 摄像机偏航角，绕y轴旋转的角度
    float Yaw;
    // 摄像机俯仰角，绕x轴旋转的角度
    float Pitch;

    // 摄像机移动速度
    float MovementSpeed;
    // zoom刻度
    float Zoom;
    // 鼠标灵敏度
    float MouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW, float pitch = PITCH) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        updateCameraVectors();

    }
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f))
        , MovementSpeed(SPEED)
        , MouseSensitivity(SENSITIVITY)
        , Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // 获得摄像机的view矩阵,锁定target
    glm::mat4 GetViewMatrix();

    /// \brief Move
    /// 摄像机移动方法
    /// \param distence 移动的距离向量
    void Move(Camera_Movement direction, float deltaTime);

    /// \brief Rotate
    /// 摄像机旋转方法
    /// \param rotate_vec3 三方向移动角度向量
    void Rotate(glm::vec3 rotateVec3);

    /// \brief Zoom
    /// 摄像机缩进拉远方法
    /// \param zoom_offset 缩放大小
    void ProcessMouseScroll(float yoffset);

    void lookToPos(glm::vec3 lookPos);

private:
    void updateCameraVectors();

};


#endif // CAMERA_H
