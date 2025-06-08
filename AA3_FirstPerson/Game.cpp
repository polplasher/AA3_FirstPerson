#include "Game.h"
#include "Player.h"
#include "Camera.h"
#include "InputManager.h"
#include "LightingSystem.h"
#include "CollisionSystem.h"
#include "Minimap.h"
#include "Renderer.h"
#include "Scene.h"

Game* Game::instance = nullptr;

Game::Game() : winWidth(800), winHeight(600), isRunning(false) {
    instance = this;
    player = new Player();
    camera = new Camera();
    inputManager = new InputManager();
    lightingSystem = new LightingSystem();
    collisionSystem = new CollisionSystem();
    minimap = new Minimap();
    renderer = new Renderer();
    scene = new Scene();
}

Game::~Game() {
    delete player;
    delete camera;
    delete inputManager;
    delete lightingSystem;
    delete collisionSystem;
    delete minimap;
    delete renderer;
    delete scene;
}

void Game::Initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("Cemetery Scene");

    glEnable(GL_DEPTH_TEST);

    lightingSystem->Initialize();
    collisionSystem->Initialize();
    scene->Initialize(collisionSystem);
    renderer->Initialize();

    glutDisplayFunc(DisplayCallback);
    glutReshapeFunc(ReshapeCallback);
    glutTimerFunc(30, TimerCallback, 0);
    glutKeyboardFunc(KeyDownCallback);
    glutKeyboardUpFunc(KeyUpCallback);

    // Hide the cursor and center
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(winWidth / 2, winHeight / 2);

    // Mouse movement callback
    glutPassiveMotionFunc([](int x, int y) {
        if (Game::instance) {
            int cx = Game::instance->winWidth / 2;
            int cy = Game::instance->winHeight / 2;
            int dx = x - cx;
            int dy = cy - y;
            Game::instance->inputManager->AddMouseDelta(dx, dy);
            glutWarpPointer(cx, cy);

        }
        });

    glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
}

void Game::Run() {
    glutMainLoop();
}

void Game::Update() {
    player->Update(inputManager, collisionSystem);
    camera->Update(player);
    minimap->Update(inputManager);
    inputManager->Update();

    glutPostRedisplay();
}

void Game::Reshape(int width, int height) {
    winWidth = width;
    winHeight = height;
    camera->SetViewport(width, height);
}

// Static callbacks
void Game::DisplayCallback() {
    if (instance) {
        instance->renderer->Render(instance->player, instance->scene,
            instance->minimap, instance->camera);
    }
}

void Game::ReshapeCallback(int width, int height) {
    if (instance) {
        instance->Reshape(width, height);
    }
}

void Game::TimerCallback(int value) {
    if (instance) {
        instance->lightingSystem->Update(0.03f, instance->player);
        instance->Update();
    }
    glutTimerFunc(30, TimerCallback, 0);
}

void Game::KeyDownCallback(unsigned char key, int x, int y) {
    if (!instance) { return; }
    instance->inputManager->KeyDown(key, x, y);

    // If we press ESC, we release the cursor and exit window mode.
    if (key == 27) { 
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        glutLeaveMainLoop();
        return;
    }

	if ((key == 'f' || key == 'F') && instance->lightingSystem->IsNight()) { // toggle flashlight only at night
        instance->lightingSystem->ToggleFlashlight();
    }
    else if ((key == 'e' || key == 'E')) { // toggle interactable lanterns
        for (auto& obj : instance->scene->GetObjects()) {
            if (obj.type == SceneObject::LANTERN) {
                float dx = obj.x - instance->player->GetX();
                float dz = obj.z - instance->player->GetZ();
                float distSq = dx * dx + dz * dz;
                if (distSq < 4.0f) { // distance < 2.0
                    obj.lightOn = !obj.lightOn;
                    if (obj.lightID >= 0) {
                        if (obj.lightOn) { glEnable(obj.lightID); }
                        else { glDisable(obj.lightID); }
                    }
                }
            }
        }
    }
}

void Game::KeyUpCallback(unsigned char key, int x, int y) {
    if (instance) { instance->inputManager->KeyUp(key, x, y); }
}