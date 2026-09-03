#include "Camera.hpp"

namespace Camera {

    Camera::Camera(glm::vec3 startPos, glm::vec3 startUp, float startYaw, float startPitch)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), 
          movementSpeed(3.0f), 
          mouseSensitivity(0.1f) 
    {
        position = startPos;
        worldUp  = startUp;
        yaw = startYaw;
        pitch = startPitch;

        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }
    

    void Camera::processKeyboard(Movement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;

        if (direction == FORWARD)  position += front * velocity;
        if (direction == BACKWARD) position -= front * velocity;
        if (direction == LEFT)     position -= right * velocity;
        if (direction == RIGHT)    position += right * velocity;
    }

    void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if (constrainPitch) {
            if (pitch > 89.0f)  pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::updateCameraVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
}