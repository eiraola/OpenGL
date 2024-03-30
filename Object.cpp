#include "Object.h"

enum EDirection { Forward, Backward, Left, Right };
Object::Object()
{
	ID = 0;
	transMatrix = glm::mat4(1.0f);
	position = glm::vec3();
	renderer = nullptr;
	children = {};
	childrenNumber = 0;
}

Object::Object(int id, Renderer* renderer)
{
	ID = id;
	transMatrix = glm::mat4(1.0f);
	position = glm::vec3();
	this->renderer = renderer;
	children = {};
	childrenNumber = 0;
}

Object::~Object()
{
	children.clear();
}

void Object::Draw(Camera* camera, glm::mat4 modelMat)
{
	if (renderer != nullptr)
	{
		renderer->Draw(camera, transMatrix, modelMat);
	}

	for (Object* child : children) {
		child->Draw(camera, modelMat * transMatrix);
	}
}

void Object::Translate(glm::vec3 direction)
{
	transMatrix = glm::translate(transMatrix, direction);
	
}

void Object::Rotate(float angle, glm::vec3 axis)
{
	
	transMatrix = glm::rotate(transMatrix, angle, axis);
}

void Object::Scale(glm::vec3 scaleValue)
{
	transMatrix = glm::scale(transMatrix, scaleValue);
}

void Object::LoadIdentity()
{
	transMatrix = glm::mat4(1.0f);
}

void Object::SetPosition(glm::vec3 newPos)
{
	LoadIdentity();
	Translate(newPos);
	position = newPos;
}

void Object::AddChild(Object* child)
{
	children.push_back(child);
}

void Object::DeleteChild(Object* child)
{
}
