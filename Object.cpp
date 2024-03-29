#include "Object.h"

enum EDirection { Forward, Backward, Left, Right };
Object::Object()
{
}
Object::Object(int id, unsigned int shaderId, float srcvertices[], int verticesSize, unsigned int srcindices[], int indicesSize, Renderer* renderer)
{
	ID = id;
	shaderID = shaderId;
	transMatrix = glm::mat4(1.0f);
	vertices = new float[verticesSize];
	std::memcpy(vertices, srcvertices, verticesSize * sizeof(float));
	indices = new unsigned int[indicesSize];
	triangleNumber = indicesSize ;
	std::memcpy(indices, srcindices, indicesSize * sizeof(unsigned int));
	position = glm::vec3();
	this->renderer = renderer;
	children = {};
	childrenNumber = 0;
	//We create our data buffers
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//We bind our VAO so we can save our vertex atributes
	glBindVertexArray(VAO);

	//We set our vertex data to the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(float), vertices, GL_STATIC_DRAW);

	//We finally set the triangle data into the EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	//Setting the vertex data attributes
		//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture atribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//We unbind the vertex array since we dont need it anymore.
	glBindVertexArray(0);
}

Object::~Object()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	children.clear();
}

void Object::Draw(Camera* camera, glm::mat4 modelMat)
{
	if (renderer != nullptr)
	{
		renderer->Draw(camera, modelMat, transMatrix);
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
