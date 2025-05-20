#pragma once
#include <GL/freeglut.h>
#include <vector>

// Forward declarations
class Player;
class Camera;
class InputManager;
class LightingSystem;
class CollisionSystem;
class Minimap;
class Renderer;
class Scene;

class Game {
private:
    int winWidth, winHeight;
    bool isRunning;

    Player* player;
    Camera* camera;
    InputManager* inputManager;
    LightingSystem* lightingSystem;
    CollisionSystem* collisionSystem;
    Minimap* minimap;
    Renderer* renderer;
    Scene* scene;

    static Game* instance;

public:
    Game();
    ~Game();

    void Initialize(int argc, char** argv);
    void Run();
    void Update();
    void Reshape(int width, int height);

    // Getters
    int GetWidth() const { return winWidth; }
    int GetHeight() const { return winHeight; }
    Player* GetPlayer() const { return player; }
    Camera* GetCamera() const { return camera; }
    InputManager* GetInputManager() const { return inputManager; }
    LightingSystem* GetLightingSystem() const { return lightingSystem; }
    CollisionSystem* GetCollisionSystem() const { return collisionSystem; }
    Minimap* GetMinimap() const { return minimap; }
    Scene* GetScene() const { return scene; }

    // Static callbacks for GLUT
    static void DisplayCallback();
    static void ReshapeCallback(int width, int height);
    static void TimerCallback(int value);
    static void KeyDownCallback(unsigned char key, int x, int y);
    static void KeyUpCallback(unsigned char key, int x, int y);

    // Singleton getter
    static Game* GetInstance() { return instance; }
};