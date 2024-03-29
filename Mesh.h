#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Mesh
{
public:

	Mesh(float* vertexData,
	int vertexDataSize,
	unsigned int* trianglesData,
	int triangleDataSize);
	~Mesh();
	void Draw();
private:
	void InitializeVertexData();
public:
	 
	float* vertexData;
	unsigned int* trianglesData;
	int vertexDataSize;
	int triangleDataSize;

	unsigned int VBO, VAO, EBO;
private:

};

