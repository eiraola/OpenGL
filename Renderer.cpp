#include "Renderer.h"

Renderer::Renderer(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	this->material = material;
}

Renderer::~Renderer()
{
}


void Renderer::Draw(Camera* camera, glm::mat4 transMatrix, glm::mat4 modelMatrix)
{
	material->SetMaterialValues();
	material->SetTransformationValues(camera, transMatrix, modelMatrix);
	mesh->Draw();
}
