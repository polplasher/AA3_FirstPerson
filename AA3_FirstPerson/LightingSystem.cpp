#include "LightingSystem.h"
#include "Player.h"
#include <cmath>

LightingSystem::LightingSystem() : sunAngle(0.0f), isNight(false), flashlightOn(false) {
}

void LightingSystem::Initialize() {
    // Ambient global light
    GLfloat ambientModel[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientModel);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // Sun light (day/night)
    glEnable(GL_LIGHT1);  // Moon light (night)
    glEnable(GL_LIGHT2);  // Flashlight (spotlight), off by default

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);

    // Sun light parameters
    GLfloat sunAmbient[] = { 0.2f, 0.2f, 0.15f, 1.0f };
    GLfloat sunDiffuse[] = { 0.9f, 0.9f, 0.7f, 1.0f };
    GLfloat sunSpecular[] = { 1.0f, 1.0f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);

    // Initial sun position
    GLfloat sunPos[] = { 10.0f, 15.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);

    // Moon light parameters
    GLfloat moonAmbient[] = { 0.02f, 0.02f, 0.05f, 1.0f };
    GLfloat moonDiffuse[] = { 0.1f, 0.1f, 0.2f, 1.0f };
    GLfloat moonSpecular[] = { 0.2f, 0.2f, 0.3f, 1.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT, moonAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, moonDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, moonSpecular);

    // Initial moon position
    GLfloat moonPos[] = { -10.0f, -15.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, moonPos);

    // Flashlight parameters
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 64.0f);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.05f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);

    GLfloat flashAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat flashDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat flashSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT2, GL_AMBIENT, flashAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, flashDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, flashSpecular);

    // Flashlight initially off
    glDisable(GL_LIGHT2);
}

void LightingSystem::Update(float deltaTime, Player* player) {
    // Increase sun angle (360° every 30 seconds)
    sunAngle += 0.36f;
    if (sunAngle >= 360.0f) sunAngle -= 360.0f;

    float rad = sunAngle * PI / 180.0f;
    float intensity = sinf(rad);  // >0 is day, <=0 is night
    if (intensity < 0.0f) intensity = 0.0f;

    // Sun light (GL_LIGHT0)
    GLfloat sunAmbient[] = { 0.2f * intensity, 0.2f * intensity, 0.15f * intensity, 1.0f };
    GLfloat sunDiffuse[] = { 0.9f * intensity, 0.9f * intensity, 0.7f * intensity, 1.0f };
    GLfloat sunSpecular[] = { 1.0f * intensity, 1.0f * intensity, 0.8f * intensity, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);

    // Sun position
    const float radius = 20.0f;
    GLfloat sunPos[] = { radius * cos(rad), radius * sin(rad), 0.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sunPos);

    // Moon light (GL_LIGHT1), opposite to the sun
    float moonRad = rad + PI;
    GLfloat moonPos[] = { radius * cos(moonRad), radius * sin(moonRad), 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, moonPos);

    // Background color changes for day/night cycle
    if (intensity <= 0.0f) {
        isNight = true;
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);  // Dark night color
    }
    else {
        isNight = false;
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);   // Sky blue day color
    }

    // Flashlight control: only active at night and if user turned it on
    if (isNight && flashlightOn) {
        glEnable(GL_LIGHT2);
        UpdateFlashlight(player);
    }
    else {
        glDisable(GL_LIGHT2);
    }
}

void LightingSystem::UpdateFlashlight(Player* player) {
    float dirX = sinf(player->GetYaw());
    float dirZ = -cosf(player->GetYaw());

    // Set flashlight position to player position
    GLfloat lightPos[] = { player->GetX(), player->GetY(), player->GetZ(), 1.0f };
    GLfloat spotDir[] = { dirX, 0.0f, dirZ };

    glLightfv(GL_LIGHT2, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spotDir);
}

void LightingSystem::ToggleFlashlight() {
    flashlightOn = !flashlightOn;
}