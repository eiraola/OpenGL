#pragma once
#include <Camera.h>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upVector, float FOV, float viewportW, float viewportH, float near, float far)
{
    view = glm::mat4(1.0f); 
    projection = glm::mat4(1.0f);
    //projection = glm::perspective(glm::radians(45.0f), 600.0f / 800.0f, 0.1f, 100.0f);
    //view = glm::translate(view, glm::vec3(0.0f, 1.0f, -3.0f));
    view = glm::lookAt(position, target, upVector);
    projection = glm::perspective(glm::radians(FOV), viewportW / viewportH, near, far);
}

Camera::~Camera()
{
    
}


