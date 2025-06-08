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
    if (instance) {
        instance->inputManager->KeyDown(key, x, y);
        if ((key == 'f' || key == 'F') && instance->lightingSystem->IsNight()) {
            instance->lightingSystem->ToggleFlashlight();
        }
    }
}

void Game::KeyUpCallback(unsigned char key, int x, int y) {
    if (instance) {
        instance->inputManager->KeyUp(key, x, y);
    }
}