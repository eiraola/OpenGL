#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Vertex {
	glm::vec3 vertexPos;
	glm::vec3 vertexNormal;
	glm::vec2 textureCoords;
	Vertex() {}
	Vertex(glm::vec3 data, glm::vec3 normal, glm::vec2 textCoord) {

		vertexPos = data;
		vertexNormal = normal;
		textureCoords = textCoord;
	}
};

class Mesh
{
public:

	Mesh(std::vector<Vertex> vertexData, std::vector<unsigned int> triangleData);
	~Mesh();
	void Draw();
private:
	void InitializeVertexData();
public:
	std::vector<Vertex> vertexData;
	std::vector<unsigned int> triangleData;
	

	unsigned int VBO, VAO, EBO;
private:

};

