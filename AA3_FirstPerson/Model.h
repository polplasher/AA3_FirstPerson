#pragma once
#include <GL/freeglut.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>

struct Mesh {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> indices;
    float color[3] = { 0.8f, 0.8f, 0.8f }; // Default color
};

class Model {
private:
    std::vector<Mesh> meshes;
    std::string directory;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

public:
    Model();
    ~Model();

    bool LoadModel(const std::string& path);
    void Draw();
    void SetColor(float r, float g, float b);
};