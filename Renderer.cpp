#include "Renderer.h"

Renderer::Renderer(Mesh* mesh, Shader* shader)
{
	this->mesh = mesh;
	this->shader = shader;
}

Renderer::~Renderer()
{
}


void Renderer::Draw(Camera* camera, glm::mat4 transMatrix, glm::mat4 modelMatrix)
{

	shader->Use();
	shader->Use();
	shader->SetMat4("model", modelMatrix);
	shader->SetMat4("transform", transMatrix);
	shader->SetMat4("view", camera->view);
	shader->SetMat4("projection", camera->projection);
	mesh->Draw();
}
