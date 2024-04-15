#include "Object.h"

enum EDirection { Forward, Backward, Left, Right };
Object::Object()
{
	ID = 0;
	renderer = nullptr;
	children = {};
	childrenNumber = 0;
}

Object::Object(int id, Renderer* renderer)
{
	ID = id;
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
		renderer->Draw(camera, transform.ModelMatrix(), modelMat);
	}

	for (Object* child : children) {
		child->Draw(camera, modelMat * transform.ModelMatrix());
	}
}


void Object::AddChild(Object* child)
{
	children.push_back(child);
}

void Object::DeleteChild(Object* child)
{
}

Transform* Object::GetTransform()
{
	return &transform;
}
