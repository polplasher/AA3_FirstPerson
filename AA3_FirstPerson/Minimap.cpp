#include "InputManager.h"
#include "Minimap.h"
#include "Player.h"
#include "Scene.h"
#include <cmath>

Minimap::Minimap() : visible(true), zoom(15.0f), minZoom(0.0f), maxZoom(15.0f),
posX(0), posY(0), width(200), height(200) {
}

void Minimap::Update(InputManager* inputManager) {
	// Toggle minimap visibility
	if (inputManager->IsKeyJustPressed('m') || inputManager->IsKeyJustPressed('M')) {
		visible = !visible;
	}

	if (!visible) return;

	// Adjust zoom
	if (inputManager->IsKeyPressed('-') && zoom < maxZoom) {
		zoom += 0.1f;
	}
	else if (inputManager->IsKeyPressed('+') && zoom > minZoom) {
		zoom -= 0.1f;
	}
}

void Minimap::Draw(Player* player, Scene* scene) const {
	if (!visible) return;

	// Set up 2D orthographic projection for minimap
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-zoom, zoom, -zoom, zoom, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-player->GetX(), -player->GetZ(), 0.0f);

	// Draw minimap background
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);
	glVertex2f(-15, -15);
	glVertex2f(15, -15);
	glVertex2f(15, 15);
	glVertex2f(-15, 15);
	glEnd();

	// Draw tombs (represented as red squares)
	// This hardcoded drawing should be replaced with accessing scene objects
	glColor3f(0.8f, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(2 - 0.5f, -2 - 0.5f);
	glVertex2f(2 + 0.5f, -2 - 0.5f);
	glVertex2f(2 + 0.5f, -2 + 0.5f);
	glVertex2f(2 - 0.5f, -2 + 0.5f);
	glEnd();

	glColor3f(0.8f, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(-3 - 0.5f, 4 - 0.5f);
	glVertex2f(-3 + 0.5f, 4 - 0.5f);
	glVertex2f(-3 + 0.5f, 4 + 0.5f);
	glVertex2f(-3 - 0.5f, 4 + 0.5f);
	glEnd();

	// Draw mausoleum (grey square)
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2f(8 - 1, 8 - 1);
	glVertex2f(8 + 1, 8 - 1);
	glVertex2f(8 + 1, 8 + 1);
	glVertex2f(8 - 1, 8 + 1);
	glEnd();

	// Draw player indicator as a triangle showing direction
	float dx = sinf(player->GetYaw()) * 2;
	float dz = -cosf(player->GetYaw()) * 2;
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(player->GetX(), player->GetZ());
	glVertex2f(player->GetX() + dx * 0.5f, player->GetZ() + dz * 0.5f);
	glVertex2f(player->GetX() - dz * 0.2f, player->GetZ() + dx * 0.2f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Minimap::SetPosition(int x, int y) {
	posX = x;
	posY = y;
}

void Minimap::SetDimensions(int w, int h) {
	width = w;
	height = h;
}

void Minimap::SetZoom(float newZoom) {
	if (newZoom >= minZoom && newZoom <= maxZoom) {
		zoom = newZoom;
	}
}