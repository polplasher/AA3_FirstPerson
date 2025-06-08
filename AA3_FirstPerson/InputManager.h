#pragma once
#include <array>

class InputManager {
private:
    std::array<bool, 256> keys;
    std::array<bool, 256> prevKeys;

    float  mouseDeltaX, mouseDeltaY;

public:
    InputManager();

    void Update();
    void KeyDown(unsigned char key, int x, int y);
    void KeyUp(unsigned char key, int x, int y);

    bool IsKeyPressed(unsigned char key) const;
    bool IsKeyJustPressed(unsigned char key) const;

    void AddMouseDelta(int dx, int dy);
    float GetMouseDeltaX() const;
    float GetMouseDeltaY() const;
    void  ClearMouseDeltas();
};