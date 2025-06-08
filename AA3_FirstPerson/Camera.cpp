#include "Camera.h"
#include "Player.h"
#include <cmath>

Camera::Camera() : viewportWidth(800), viewportHeight(600) {
}

void Camera::SetViewport(int width, int height) {
    if (height == 0) height = 1;

    viewportWidth = width;
    viewportHeight = height;

    SetupPerspective(width, height);
}

void Camera::SetupPerspective(int width, int height) {
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)width / (double)height, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

void Camera::Update(Player* player) {
    // Camera updates with player position
}

void Camera::ApplyTransform(Player* player) {
    glLoadIdentity();
    float yaw = player->GetYaw();
    float pitch = player->GetPitch();
    float cosP = cosf(pitch);

    float dirX = cosP * sinf(yaw);
    float dirY = sinf(pitch);
    float dirZ = -cosP * cosf(yaw);

    gluLookAt(
        player->GetX(), player->GetY(), player->GetZ(),
        player->GetX() + dirX,
        player->GetY() + dirY,
        player->GetZ() + dirZ,
        0.0f, 1.0f, 0.0f
    );
}
