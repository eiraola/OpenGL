#pragma once
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath);
    void Use();
    void SetBool(const std::string& name, bool value);
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);

    void SetVec3(const std::string& name, float x, float y, float z);

    void SetVec4(const std::string& name, float x, float y, float z, float w);

    void SetMat4(const std::string& name, glm::mat4 matrix);

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};