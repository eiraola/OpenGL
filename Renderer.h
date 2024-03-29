#pragma once
#include <Mesh.h>
#include <Shader.h>
#include <Camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Renderer
{
public:
	Renderer(Mesh* mesh, Shader* shader);
	~Renderer();
	void Draw(class Camera* camera, glm::mat4 transMatrix, glm::mat4 modelMatrix);
public:
	Mesh* mesh;
	Shader* shader;
};

