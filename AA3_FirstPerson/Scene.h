#pragma once
#include <GL/freeglut.h>
#include <vector>

class CollisionSystem;

struct SceneObject {
    enum Type { TOMB, MAUSOLEUM, LANTERN, BUSH, PATH };
    Type type;
    float x, z;
    float width, height, depth;
};

class Scene {
private:
    std::vector<SceneObject> objects;

public:
    Scene();

    void Initialize(CollisionSystem* collisionSystem);
    void Draw();

    void DrawGround();
    void DrawFence();
    void DrawPath(float x, float z, float width, float length);
    void DrawTomb(float x, float z);
    void DrawMausoleum(float x, float z);
    void DrawLantern(float x, float z);
    void DrawBush(float x, float z);

    const std::vector<SceneObject>& GetObjects() const { return objects; }
};