#include "InputManager.h"

InputManager::InputManager() {
    keys.fill(false);
    prevKeys.fill(false);
    mouseDeltaX = mouseDeltaY = 0.0f;
}

void InputManager::KeyDown(unsigned char key, int x, int y) {
    keys[key] = true;
}

void InputManager::KeyUp(unsigned char key, int x, int y) {
    keys[key] = false;
}

void InputManager::Update() {
    // Copy current key states to previous
    for (int i = 0; i < 256; i++) {
        prevKeys[i] = keys[i];
    }

    ClearMouseDeltas();
}

void InputManager::AddMouseDelta(int dx, int dy) {
    mouseDeltaX += dx;
    mouseDeltaY += dy;
}

bool InputManager::IsKeyPressed(unsigned char key) const {
    return keys[key];
}

bool InputManager::IsKeyJustPressed(unsigned char key) const {
    return keys[key] && !prevKeys[key];
}

float InputManager::GetMouseDeltaX() const { return mouseDeltaX; }
float InputManager::GetMouseDeltaY() const { return mouseDeltaY; }
void  InputManager::ClearMouseDeltas() { mouseDeltaX = mouseDeltaY = 0.0f; }