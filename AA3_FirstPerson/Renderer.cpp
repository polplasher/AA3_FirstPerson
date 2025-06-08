#include "Renderer.h"
#include "Player.h"
#include "Scene.h"
#include "Minimap.h"
#include "Camera.h"
#include "DayCycle.h"

Renderer::Renderer() {
}

void Renderer::Initialize() {
	// Any renderer-specific initialization
}

void Renderer::Render(Player* player, Scene* scene, Minimap* minimap, Camera* camera, DayCycle* dayCycle) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set up main camera view
	camera->ApplyTransform(player);

	// Draw the scene
	scene->Draw();
	dayCycle->Draw();

	// Draw minimap in its own viewport
	int winW = glutGet(GLUT_WINDOW_WIDTH);
	int winH = glutGet(GLUT_WINDOW_HEIGHT);

	glViewport(winW - 200, winH - 200, 200, 200);
	minimap->Draw(player, scene);


	// Restore default viewport
	glViewport(0, 0, winW, winH);

	glutSwapBuffers();
}

void Renderer::SetViewport(int x, int y, int width, int height) {
	glViewport(x, y, width, height);
}

void Renderer::RestoreDefaultViewport(int width, int height) {
	glViewport(0, 0, width, height);
}