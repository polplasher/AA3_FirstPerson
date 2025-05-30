#pragma once
#include <GL/freeglut.h>
#include <vector>
#include <map>
#include <string>
#include "Model.h"

class CollisionSystem;

struct SceneObject {
    enum Type { TOMB, MAUSOLEUM, LANTERN, BUSH, PATH, MODEL };
    Type type;
    float x, z;
    float width, height, depth;
    float rotationY = 0.0f;
    Model* model = nullptr;
    std::string modelPath;
};

class Scene {
private:
    std::vector<SceneObject> objects;
    std::map<std::string, Model*> modelCache; // Cache loaded models

public:
    Scene();
    ~Scene();

    void Initialize(CollisionSystem* collisionSystem);
    void Draw();

    void DrawGround();
    void DrawFence();
    void DrawPath(float x, float z, float width, float length);
    void DrawTomb(float x, float z);
    void DrawMausoleum(float x, float z);
    void DrawLantern(float x, float z);
    void DrawBush(float x, float z);
    void DrawModel(const SceneObject& obj);

    // New method to add a 3D model to the scene
    void AddModel(const std::string& modelPath, float x, float z, float scale = 1.0f,
        float rotY = 0.0f, float r = 0.7f, float g = 0.7f, float b = 0.7f);

    const std::vector<SceneObject>& GetObjects() const { return objects; }
};