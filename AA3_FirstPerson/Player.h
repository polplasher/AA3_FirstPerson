#pragma once
#include <GL/freeglut.h>

class InputManager;
class CollisionSystem;

class Player {
private:
    float x, y, z;      // position
    float yaw;          // horizontal angle in radians
    float moveSpeed;    // movement speed
    float rotSpeed;     // rotation speed
    float pitch;

public:
    Player();

    void Update(InputManager* inputManager, CollisionSystem* collisionSystem);

    // Getters
    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetZ() const { return z; }
    float GetYaw() const { return yaw; }
    float GetMoveSpeed() const { return moveSpeed; }
    float GetRotSpeed() const { return rotSpeed; }
    float GetPitch() const { return pitch; }

    // Setters
    void SetPosition(float newX, float newY, float newZ);
    void SetYaw(float newYaw) { yaw = newYaw; }
};