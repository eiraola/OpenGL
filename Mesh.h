#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	Vertex() {
	
	};
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoords) {
		Position = position;
		Normal = normal;
		TexCoords = texCoords;
	}
};

class Mesh
{
public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	~Mesh();
	void Draw();
private:
	void InitializeVertexData();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	unsigned int VBO, VAO, EBO;
private:

};

