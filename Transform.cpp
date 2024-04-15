#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

void Transform::Translate(glm::vec3 direction)
{
	translationMatrix = glm::translate(translationMatrix,direction);
}

void Transform::Rotate(float angle, glm::vec3 axis)
{
	rotationMatrix = glm::rotate(rotationMatrix, angle, axis);
}

void Transform::Scale(glm::vec3 sclaeValue)
{
	scalationMatrix = glm::scale(scalationMatrix, sclaeValue);
}

void Transform::LoadIdentity()
{
}

void Transform::SetLocalPosition(glm::vec3 newPos)
{
	translationMatrix = glm::mat4(1.0f);
	Translate(newPos);
}

void Transform::SetLocalRotation(glm::vec3 newRotation)
{
	rotationMatrix = glm::mat4(1.0f);
	Rotate(glm::length(newRotation), glm::normalize(newRotation));
}

void Transform::SetLocalScale(glm::vec3 newScale)
{
	scalationMatrix = glm::mat4(1.0f);
	Scale(newScale);
}

glm::mat4 Transform::ModelMatrix()
{
	return translationMatrix * rotationMatrix * scalationMatrix;
}
