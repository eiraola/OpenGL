#include "ModelImporter.h"
#include <stb_image.h>

ModelImporter::ModelImporter()
{
}

Object* ModelImporter::LoadModel(std::string path, Shader* shader)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    Object* root = new Object();
    root->AddChild(ProcessNode(scene->mRootNode, scene, path.substr(0, path.find_last_of('/')), shader));
	return root;
}

Object* ModelImporter::ProcessNode(aiNode* node, const aiScene* scene, std::string path, Shader* shader)
{
    Object* parent = new Object();
    
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        Mesh* mesh = ProcessMesh(aiMesh, scene);
        Material* material = ProcessMaterial(aiMesh, scene, path);
        Renderer* renderer = new Renderer(mesh, material);
        material->shader = shader;
        Object* child = new Object();
        child->renderer = renderer;
        parent->AddChild(child);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        parent->AddChild(ProcessNode(node->mChildren[i], scene, path, shader));
    }
    return parent;
}

Mesh* ModelImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; 
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
            // tangent
           // vector.x = mesh->mTangents[i].x;
           // vector.y = mesh->mTangents[i].y;
           // vector.z = mesh->mTangents[i].z;
           // vertex.Tangent = vector;
           // // bitangent
           // vector.x = mesh->mBitangents[i].x;
           // vector.y = mesh->mBitangents[i].y;
           // vector.z = mesh->mBitangents[i].z;
           // vertex.Bitangent = vector;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return new Mesh(vertices, indices);
}

Material* ModelImporter::ProcessMaterial(aiMesh* mesh, const aiScene* scene, std::string path)
{
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    unsigned int diffuseMap = 0;
    bool usesDiffuseMap = false;
    unsigned int specularMap = 0;
    bool usesSpecularMap = false;
    glm::vec3 specularValue(0.5f, 0.5f, 0.5f);
    glm::vec3 ambientValue(0.1f, 0.1f, 0.1f);
    glm::vec3 diffuseValue(0.9f, 0.1f, 0.1f);
    float shininessValue = 0.03f;

    if (material->GetTextureCount(aiTextureType_DIFFUSE) >= 0)
    {
        usesDiffuseMap = true;
        diffuseMap = LoadTexture(material, aiTextureType_DIFFUSE, path);
    }

    if (material->GetTextureCount(aiTextureType_SPECULAR) >= 0)
    {
        usesSpecularMap = true;
        specularMap = LoadTexture(material, aiTextureType_SPECULAR, path);
    }
    
    return new Material(diffuseMap, usesDiffuseMap, specularMap, usesSpecularMap, specularValue, ambientValue,diffuseValue, shininessValue);
}

unsigned int ModelImporter::LoadTexture(aiMaterial* mat, aiTextureType type, std::string path)
{
    if (mat->GetTextureCount(type) <= 0)
    {
        return 0;
    } 
    aiString str;
    mat->GetTexture(type, 0, &str);
    unsigned int id = TextureFromFile(str.C_Str(), path, false);

    return id;
}

unsigned int ModelImporter::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    if (loadedTexts.find(filename) != loadedTexts.end())
    {
        
        return loadedTexts[filename];
    }
    std::cout << "Process Texture " << path << std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RED;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    loadedTexts[filename] = textureID;
    return textureID;
}
