#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Transform
{
public:
	Transform();
	~Transform();
public:
	void Translate(glm::vec3 direction);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 sclaeValue);
	void LoadIdentity();
	void SetLocalPosition(glm::vec3 newPos);
	void SetLocalRotation(glm::vec3 newPos);
	void SetLocalScale(glm::vec3 newPos);
	glm::mat4 ModelMatrix();
private:
	glm::mat4 translationMatrix;
	glm::mat4 rotationMatrix;
	glm::mat4 scalationMatrix;
};

