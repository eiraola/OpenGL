#include "Snake.h"

Snake::Snake(Object* snakeMesh, glm::vec3 startPos)
{
	mesh = snakeMesh;
	startPosition = startPos;
	currentPos = startPos;
	currentDirection = EDirection::Forward;
	child = nullptr;
    moveMag = 0.2f;
}

Snake::~Snake()
{
    if (child != nullptr)
    {
        delete child;
    }
}

void Snake::ChangeDir(EDirection newDirection)
{
	currentDirection = newDirection;
}

void Snake::Advance()
{
    glm::vec3 increment = glm::vec3();
    switch (currentDirection) 
    {
    case EDirection::Forward: 
        increment = glm::vec3(0.0f, moveMag, 0.0f);
        break;
    case EDirection::Back:
        increment = glm::vec3(0.0f, -moveMag, 0.0f);
        break;
    case EDirection::Left:
        increment = glm::vec3(-moveMag, 0.0f, 0.0f);
        break;
    case EDirection::Right:
        increment = glm::vec3(moveMag, 0.0f, 0.0f);
        break;
    }

    if (child!= nullptr)
    {
        child->Follow(currentPos);
    }
    currentPos = currentPos + increment;

}

void Snake::Follow(glm::vec3 parentPos)
{
    if (child != nullptr)
    {
        child->Follow(currentPos);
    }
    currentPos = parentPos;
}

void Snake::Draw()
{
    if (child != nullptr)
    {
        child->Draw();
    }
    mesh->LoadIdentity();
    mesh->Translate(currentPos);
    mesh->Draw();
}

void Snake::Grow()
{
    if (child != nullptr)
    {
        child->Grow();
        return;
    }
    child = new Snake(mesh, currentPos);
}
