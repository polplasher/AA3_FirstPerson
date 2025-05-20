#include "Player.h"
#include "InputManager.h"
#include "CollisionSystem.h"
#include <cmath>

Player::Player() : x(0.0f), y(1.0f), z(5.0f), yaw(0.0f), moveSpeed(0.1f), rotSpeed(0.03f) {
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
    if (inputManager->IsKeyPressed('d') || inputManager->IsKeyPressed('D')) {
        yaw += rotSpeed;
    }
    if (inputManager->IsKeyPressed('a') || inputManager->IsKeyPressed('A')) {
        yaw -= rotSpeed;
    }

    // Check for collisions before moving
    float newX = x + dx;
    float newZ = z + dz;
    if (!collisionSystem->IsColliding(newX, newZ)) {
        x = newX;
        z = newZ;
    }
}