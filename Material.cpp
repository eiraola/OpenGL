#include "Material.h"

Material::Material(unsigned int diffuseMap, bool usesDiffuseMap, unsigned int specularMap, bool usesSpecularMap, glm::vec3 ambientValue, glm::vec3 diffuseValue, float shininessValue)
{
	this->diffuseMap = diffuseMap;
	this->usesDiffuseMap = usesDiffuseMap;
	this->specularMap = specularMap;
	this->usesSpecularMap = usesSpecularMap;
	this->ambientValue = ambientValue;
	this->diffuseValue = diffuseValue;
	this->shininessValue = shininessValue;
	shader = nullptr;
}

Material::~Material()
{
}

void Material::SetMaterialValues()
{
	if (shader == nullptr)
	{
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	shader->Use();
	shader->SetVec3("material.ambient", ambientValue.x, ambientValue.y, ambientValue.z);
	shader->SetVec3("material.diffuse", diffuseValue.x, diffuseValue.y, diffuseValue.z);
	shader->SetInt("material.diffuseMap", 0);
	shader->SetInt("material.specular", 1); 
	shader->SetFloat("material.shininess", shininessValue);
	shader->SetBool("material.useDiffuseMap", usesDiffuseMap);
	shader->SetBool("material.useSpecularMap", usesSpecularMap);
}

void Material::SetTransformationValues(Camera* camera, glm::mat4 transMatrix, glm::mat4 modelMatrix)
{
	shader->Use();
	shader->SetMat4("model", modelMatrix);
	shader->SetMat4("transform", transMatrix);
	shader->SetMat4("view", camera->view);
	shader->SetMat4("projection", camera->projection);
}
