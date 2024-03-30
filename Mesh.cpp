#include "Mesh.h"

Mesh::Mesh(float* vertexData, int vertexDataSize, unsigned int* trianglesData, int triangleDataSize)
{
	this->vertexDataSize = vertexDataSize;
	this->triangleDataSize = triangleDataSize;
	this->vertexData = new float[this->vertexDataSize];
	this->trianglesData = new unsigned int[this->triangleDataSize];

	std::memcpy(this->vertexData, vertexData, vertexDataSize * sizeof(float));
	std::memcpy(this->trianglesData, trianglesData, triangleDataSize * sizeof(unsigned int));

	//We create our data buffers
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	InitializeVertexData();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, triangleDataSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::InitializeVertexData()
{
	//We bind our VAO so we can save our vertex atributes
	glBindVertexArray(VAO);

	//We set our vertex data to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize * sizeof(float), vertexData, GL_STATIC_DRAW);

	//We finally set the triangle data into the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangleDataSize * sizeof(unsigned int), trianglesData, GL_STATIC_DRAW);

	//Setting the vertex data attributes

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Texture atribute
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//We unbind the vertex array since we dont need it anymore.
	glBindVertexArray(0);
}
