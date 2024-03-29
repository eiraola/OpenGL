#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
enum EProjectionType {
    Perspective,
    Ortho
};
class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upVector, float FOV, float viewportH, float viewportW, float near, float far);
    ~Camera();
public:
    glm::mat4 view;
    glm::mat4 projection;
};

