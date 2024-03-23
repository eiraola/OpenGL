#pragma once
#include <Object.h>
enum EDirection { Forward, Left, Right, Back };
class Snake
{
public:
	Snake(Object* snakeMesh, glm::vec3 startPos);
	~Snake();
	void ChangeDir(EDirection newDirection);
	void Advance();
	void Follow(glm::vec3 parentPos);
	void Draw();
	void Grow();

public:
	Object* mesh;
	glm::vec3 startPosition;
	glm::vec3 currentPos;
	EDirection currentDirection;
	Snake* child;
	float moveMag;
};

