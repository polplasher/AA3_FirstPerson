#include "Player.h"
#include "InputManager.h"
#include "CollisionSystem.h"
#include <cmath>

Player::Player() : x(0.0f), y(1.0f), z(5.0f), yaw(0.0f), pitch(0), moveSpeed(0.1f), rotSpeed(0.03f) {
}

void Player::SetPosition(float newX, float newY, float newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

void Player::Update(InputManager* inputManager, CollisionSystem* collisionSystem) {
    float dx = 0.0f, dz = 0.0f;

    // Handle movement based on input
    if (inputManager->IsKeyPressed('w') || inputManager->IsKeyPressed('W')) {
        dx += sinf(yaw) * moveSpeed;
        dz += -cosf(yaw) * moveSpeed;
    }
    if (inputManager->IsKeyPressed('s') || inputManager->IsKeyPressed('S')) {
        dx += -sinf(yaw) * moveSpeed;
        dz += cosf(yaw) * moveSpeed;
    }
    if (inputManager->IsKeyPressed('a') || inputManager->IsKeyPressed('A')) {
        dx += -cosf(yaw) * moveSpeed;
        dz += -sinf(yaw) * moveSpeed;
    }
    if (inputManager->IsKeyPressed('d') || inputManager->IsKeyPressed('D')) {
        dx += cosf(yaw) * moveSpeed;
        dz += sinf(yaw) * moveSpeed;
    }

    if (inputManager->IsKeyPressed('e') || inputManager->IsKeyPressed('E')) {
        yaw += rotSpeed;
    }
    if (inputManager->IsKeyPressed('q') || inputManager->IsKeyPressed('Q')) {
        yaw -= rotSpeed;
    }

    // Quick turn with the mouse
    yaw += inputManager->GetMouseDeltaX() * 0.005f;
    pitch += inputManager->GetMouseDeltaY() * 0.005f;
    // Clamp pitch
    if (pitch < -1.5f)      pitch = -1.5f;
    else if (pitch > 1.5f) pitch = 1.5f;

    // Check for collisions before moving
    float newX = x + dx;
    float newZ = z + dz;
    if (!collisionSystem->IsColliding(newX, newZ)) {
        x = newX;
        z = newZ;
    }

    inputManager->ClearMouseDeltas();
}