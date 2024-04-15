#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <Camera.h>
class Material
{
public:
	Material(unsigned int diffuseMap, 
        bool usesDiffuseMap, 
        unsigned int specularMap, 
        bool usesSpecularMap, 
        glm::vec3 ambientValue, 
        glm::vec3 diffuseValue, 
        float shininessValue);
	~Material();
    void SetMaterialValues();
    void SetTransformationValues(Camera* camera, glm::mat4 transMatrix, glm::mat4 modelMatrix);
public:
    Shader* shader;
    unsigned int diffuseMap;
    bool usesDiffuseMap;
    unsigned int specularMap;
    bool usesSpecularMap;
    glm::vec3 ambientValue;
    glm::vec3 diffuseValue;
    float shininessValue;

};

