#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "game.h"

std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& directory);
void process_node(aiNode* node, const aiScene* scene, Model& obj);
Mesh process_mesh(aiMesh* mesh, const aiScene* scene, Model& model);

void load_obj(Mesh& obj, std::string const & path) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    obj.directory = path.substr(0, path.find_last_of('/'));

    aiMesh* mesh = scene->mMeshes[0];

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        obj.vertices.push_back(mesh->mVertices[i].x);
        obj.vertices.push_back(mesh->mVertices[i].y);
        obj.vertices.push_back(mesh->mVertices[i].z);

        obj.normals.push_back(mesh->mNormals[i].x);
        obj.normals.push_back(mesh->mNormals[i].y);
        obj.normals.push_back(mesh->mNormals[i].z);

        obj.texCoords.push_back(mesh->mTextureCoords[0][i].x);
        obj.texCoords.push_back(mesh->mTextureCoords[0][i].y);



    }
    std::cout << mesh->mNumVertices << " vertices loaded\n";
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            obj.indices.push_back(face.mIndices[j]);
    }
    std::cout << mesh->mNumFaces << " faces loaded\n";
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse", obj.directory);
        obj.textures.insert(obj.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular", obj.directory);
        obj.textures.insert(obj.textures.end(), specularMaps.begin(), specularMaps.end());
    }
    
}

void load_model(Model& obj, std::string const& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    obj.directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene, obj);


}

void process_node(aiNode* node, const aiScene* scene, Model& obj) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        obj.meshes.push_back(process_mesh(mesh, scene, obj));
    }
    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene, obj);
    }
}

Mesh process_mesh(aiMesh* mesh, const aiScene* scene, Model& model) {


    Mesh obj;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        obj.vertices.push_back(mesh->mVertices[i].x);
        obj.vertices.push_back(mesh->mVertices[i].y);
        obj.vertices.push_back(mesh->mVertices[i].z);

        obj.normals.push_back(mesh->mNormals[i].x);
        obj.normals.push_back(mesh->mNormals[i].y);
        obj.normals.push_back(mesh->mNormals[i].z);

        obj.texCoords.push_back(mesh->mTextureCoords[0][i].x);
        obj.texCoords.push_back(mesh->mTextureCoords[0][i].y);



    }
    std::cout << mesh->mNumVertices << " vertices loaded\n";
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            obj.indices.push_back(face.mIndices[j]);
    }
    std::cout << mesh->mNumFaces << " faces loaded\n";
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
            aiTextureType_DIFFUSE, "texture_diffuse", model.directory);
        obj.textures.insert(obj.textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Texture> specularMaps = loadMaterialTextures(material,
            aiTextureType_SPECULAR, "texture_specular", model.directory);
        obj.textures.insert(obj.textures.end(), specularMaps.begin(), specularMaps.end());
    }

    obj.generate_buffer_EBO();

    return obj;
}


std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string & directory)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::string filename = directory + '/' + std::string(str.C_Str());
        Texture texture(filename.c_str());
        std::cout << filename.c_str() << std::endl;
        textures.push_back(texture);
    }
    //std::cout << textures.size() << " materials loaded\n";
    return textures;
}