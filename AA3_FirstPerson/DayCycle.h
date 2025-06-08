#pragma once

class DayCycle {
private:
	float sunPosition[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float moonPosition[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float sunColor[3] = { 1.0f, 1.0f, 0.0f };
	float moonColor[3] = { 0.1f, 0.1f, 0.4f };

	float currentTime = 0; // Current time in seconds
	float moonAngle, sunAngle; // Angles for moon and sun

	void DrawSun();
	void DrawMoon();

public:
	DayCycle();

	void Update(float deltaTime);
	void Draw();
};