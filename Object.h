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
class Object
{
public:
	Object();
	Object(int id, Renderer* renderer);
	~Object();
	void Draw(Camera* camera, glm::mat4 modelMat);
	void Translate(glm::vec3 direction);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 sclaeValue);
	void LoadIdentity();
	void SetPosition(glm::vec3 newPos);
	void AddChild(Object* child);
	void DeleteChild(Object* child);
public:

	int ID;
	glm::mat4 transMatrix;
	glm::vec3 position;
	std::vector<Object*> children;
	Renderer* renderer;
	int childrenNumber;
};

