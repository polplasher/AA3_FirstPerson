#pragma once
#include <GL/freeglut.h>

class Player;

class Camera {
private:
    int viewportWidth, viewportHeight;

public:
    Camera();

    void Update(Player* player);
    void SetupPerspective(int width, int height);
    void ApplyTransform(Player* player);

    // Setters
    void SetViewport(int width, int height);
};