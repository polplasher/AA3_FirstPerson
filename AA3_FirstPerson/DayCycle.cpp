#include "DayCycle.h"
#include <GL/freeglut.h>
#include <cmath>

const float PI = 3.1415f;
const float SUN_DISTANCE = 20;

DayCycle::DayCycle() {
}

void DayCycle::Update(float deltaTime) {
	// Update the time of day
	currentTime += deltaTime;
	// Loop the time of day if it exceeds 24 hours
	if (currentTime >= 30.0f) {
		currentTime = 0;
	}
	sunAngle = (currentTime / 30.0f) * 360.0f; // Convert time to angle (0-360 degrees)
	moonAngle = sunAngle + 180.0f; // Moon is always opposite the sun
}

void DayCycle::Draw() {
	DrawSun();
	DrawMoon();
}

void DayCycle::DrawSun() {
	glPushMatrix();

	// Calculate the sun's orbit position
	sunPosition[0] = cos((sunAngle * PI) / 180.0f) * SUN_DISTANCE; // x
	sunPosition[1] = sin((sunAngle * PI) / 180.0f) * SUN_DISTANCE; // y
	glTranslatef(sunPosition[0], sunPosition[1], 0.0f);

	// Disable lighting for the sun itself (it's a light source)
	glDisable(GL_LIGHTING);

	glColor3f(sunColor[0], sunColor[1], sunColor[2]);
	glutSolidSphere(2, 20, 20);

	// Re-enable lighting
	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void DayCycle::DrawMoon() {
	glPushMatrix();

	// Calculate the moon's orbit position
	moonPosition[0] = cos(((sunAngle - 180) * PI) / 180.0f) * SUN_DISTANCE; // x
	moonPosition[1] = sin(((sunAngle - 180) * PI) / 180.0f) * SUN_DISTANCE; // y
	glTranslatef(moonPosition[0], moonPosition[1], 0.0f);

	// Disable lighting for the moon itself (it's a light source)
	glDisable(GL_LIGHTING);

	glColor3f(moonColor[0], moonColor[1], moonColor[2]);
	glutSolidSphere(1, 20, 20);

	// Re-enable lighting
	glEnable(GL_LIGHTING);

	glPopMatrix();
}