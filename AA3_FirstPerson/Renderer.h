#pragma once
#include <GL/freeglut.h>

class Player;
class Scene;
class Minimap;
class Camera;
class DayCycle;

class Renderer {
public:
	Renderer();

	void Initialize();
	void Render(Player* player, Scene* scene, Minimap* minimap, Camera* camera, DayCycle* dayCycle);

	void SetViewport(int x, int y, int width, int height);
	void RestoreDefaultViewport(int width, int height);
};