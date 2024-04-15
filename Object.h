#pragma once
#include <iterator>
#include <iostream>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include<array> 
#include <Renderer.h>
#include "Camera.h"
#include <Transform.h>
class Object
{
public:
	Object();
	Object(int id, Renderer* renderer);
	~Object();
	void Draw(Camera* camera, glm::mat4 modelMat);
	void AddChild(Object* child);
	void DeleteChild(Object* child);
	Transform* GetTransform();
public:

	int ID;
	std::vector<Object*> children;
	Renderer* renderer;
	int childrenNumber;
private:
	Transform transform;
};

