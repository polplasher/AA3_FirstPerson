#pragma once
#include <GL/freeglut.h>

class Player;
class Scene;
class InputManager;

class Minimap {
private:
    bool visible;
    float zoom;
    float minZoom, maxZoom;

    int posX, posY;
    int width, height;

public:
    Minimap();

    void Update(InputManager* inputManager);
    void Draw(Player* player, Scene* scene) const;

    void SetPosition(int x, int y);
    void SetDimensions(int width, int height);

    bool IsVisible() const { return visible; }
    void SetVisible(bool value) { visible = value; }

    float GetZoom() const { return zoom; }
    void SetZoom(float newZoom);
};