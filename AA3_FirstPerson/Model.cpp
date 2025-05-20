#include "Model.h"
#include <iostream>

Model::Model() {
}

Model::~Model() {
	// No textures to clean up
}

bool Model::LoadModel(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return false;
	}

	// Store the directory path
	directory = path.substr(0, path.find_last_of('/'));
	if (directory.empty()) {
		directory = path.substr(0, path.find_last_of('\\'));
	}

	processNode(scene->mRootNode, scene);
	return true;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// Process meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// Process child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	Mesh result;

	// Process vertices
	result.vertices.reserve(mesh->mNumVertices * 3);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		result.vertices.push_back(mesh->mVertices[i].x);
		result.vertices.push_back(mesh->mVertices[i].y);
		result.vertices.push_back(mesh->mVertices[i].z);
	}

	// Process normals
	if (mesh->HasNormals()) {
		result.normals.reserve(mesh->mNumVertices * 3);
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			result.normals.push_back(mesh->mNormals[i].x);
			result.normals.push_back(mesh->mNormals[i].y);
			result.normals.push_back(mesh->mNormals[i].z);
		}
	}

	// Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			result.indices.push_back(face.mIndices[j]);
		}
	}

	// Get material color if available
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiColor3D color(0.8f, 0.8f, 0.8f); // Default color
		if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
			result.color[0] = color.r;
			result.color[1] = color.g;
			result.color[2] = color.b;
		}
	}

	return result;
}

void Model::Draw() {
	for (const auto& mesh : meshes) {
		// Set color
		glColor3f(mesh.color[0], mesh.color[1], mesh.color[2]);

		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < mesh.indices.size(); i++) {
			unsigned int index = mesh.indices[i];

			// Set normal if available
			if (!mesh.normals.empty()) {
				glNormal3f(
					mesh.normals[index * 3],
					mesh.normals[index * 3 + 1],
					mesh.normals[index * 3 + 2]
				);
			}

			// Set vertex position
			glVertex3f(
				mesh.vertices[index * 3],
				mesh.vertices[index * 3 + 1],
				mesh.vertices[index * 3 + 2]
			);
		}
		glEnd();
	}
}

void Model::SetColor(float r, float g, float b) {
	for (auto& mesh : meshes) {
		mesh.color[0] = r;
		mesh.color[1] = g;
		mesh.color[2] = b;
	}
}