#pragma once
#include <GL/freeglut.h>

class Player;

class LightingSystem {
private:
    float sunAngle;
    bool isNight;
    bool flashlightOn;
    const float PI = 3.14159265f;

public:
    LightingSystem();

    void Initialize();
    void Update(float deltaTime, Player* player);
    void UpdateDayNightCycle();
    void UpdateFlashlight(Player* player);

    void ToggleFlashlight();

    bool IsNight() const { return isNight; }
    bool IsFlashlightOn() const { return flashlightOn; }
};