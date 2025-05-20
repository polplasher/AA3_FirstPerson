#pragma once
#include <array>

class InputManager {
private:
    std::array<bool, 256> keys;
    std::array<bool, 256> prevKeys;

public:
    InputManager();

    void Update();
    void KeyDown(unsigned char key, int x, int y);
    void KeyUp(unsigned char key, int x, int y);

    bool IsKeyPressed(unsigned char key) const;
    bool IsKeyJustPressed(unsigned char key) const;
};