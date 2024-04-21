#pragma once
#include <string>
#include <Mesh.h>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <Object.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>

class ModelImporter
{
public:
    ModelImporter();
    Object* LoadModel(std::string path, Shader* shader);
private:

    std::map<std::string, unsigned int> loadedTexts;
    Object* ProcessNode(aiNode* node, const aiScene* scene, std::string path, Shader* shader);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    Material* ProcessMaterial(aiMesh* mesh, const aiScene* scene, std::string path);
    unsigned int LoadTexture(aiMaterial* mat, aiTextureType type, std::string path);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

