#include <cmath>
#include <GL/freeglut.h>
#include <vector>

// structs
struct AABB { float minX, maxX, minZ, maxZ; };
struct Wall { float x, y, z, sx, sy, sz; };
struct { float x, z; } Tomb[] = {
	{  2.0f, -2.0f },
	{ -3.0f,  4.0f }
};

// game constants
const float moveSpeed = 0.1f;
const float rotSpeed = 0.03f;

// game variables
int winW = 800, winH = 600;             // window size
float px = 0.0f, py = 1.0f, pz = 5.0f;  // player pos
float yaw = 0.0f;                       // horizontal angle in radians

bool prevKeys[256] = { false };          // previous key state
bool keys[256] = { false };             // key state

std::vector<AABB> obstacles;            // obstacles for AABB collision

// minimap variables
float minimapZoom = 15.0f;              // minimap zoom level
float minZoom = 0.0f, maxZoom = 15.0f;  // minimap zoom limits
bool minimapVisible = true;             // minimap visibility

// day night
float sunAngle = 0.0f;  // Ángulo del sol para el ciclo día/noche
const float PI = 3.14159265f;
bool isNight = false;   // Para controlar si es de noche o día
bool flashlightOn = false; // Linterna activable por tecla

void InitLighting() {
	// Luz ambiental global tenue
	GLfloat ambientModel[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientModel);

	// Habilitar iluminación y luces
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);  // Luz solar (día/noche)
	glEnable(GL_LIGHT1);  // Luz lunar (noche)
	glEnable(GL_LIGHT2);  // Linterna (spotlight), apagada por defecto

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);

	// Parámetros iniciales luz solar (serán actualizados por timer)
	GLfloat sunAmbient[] = { 0.2f, 0.2f, 0.15f, 1.0f };
	GLfloat sunDiffuse[] = { 0.9f, 0.9f, 0.7f, 1.0f };
	GLfloat sunSpecular[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);
	// Posición inicial sol (temporal, se actualizará en timer)
	GLfloat sunPos[] = { 10.0f, 15.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);

	// Parámetros luz lunar tenue (GL_LIGHT1)
	GLfloat moonAmbient[] = { 0.02f, 0.02f, 0.05f, 1.0f };
	GLfloat moonDiffuse[] = { 0.1f, 0.1f, 0.2f, 1.0f };
	GLfloat moonSpecular[] = { 0.2f, 0.2f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, moonAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, moonDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, moonSpecular);
	// Posición inicial luna
	GLfloat moonPos[] = { -10.0f, -15.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, moonPos);

	// Parámetros linterna (GL_LIGHT2) tipo foco (spotlight)
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

	// Linterna inicialmente apagada, se activará solo de noche y si el jugador la enciende
	glDisable(GL_LIGHT2);
}

bool IsColliding(float p_objectX, float p_objectZ) {
	for (AABB& obst : obstacles) {
		if (p_objectX > obst.minX && p_objectX < obst.maxX && p_objectZ > obst.minZ && p_objectZ < obst.maxZ) { return true; }
	}

	return false;
}

void KeyDown(unsigned char key, int x, int y) {
	keys[key] = true;

	if (key == 'l' || key == 'L') {  // tecla L para linterna
		if (isNight) flashlightOn = !flashlightOn;
	}
}

#pragma region Drawables
void DrawPath(float p_x, float p_z, float p_width, float p_length) {
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_QUADS);

	glVertex3f(p_x - p_width / 2, 0.01f, p_z - p_length / 2);
	glVertex3f(p_x + p_width / 2, 0.01f, p_z - p_length / 2);
	glVertex3f(p_x + p_width / 2, 0.01f, p_z + p_length / 2);
	glVertex3f(p_x - p_width / 2, 0.01f, p_z + p_length / 2);

	glEnd();
}

void DrawTomb(float p_x, float p_z) {
	// base tombstone
	glColor3f(0.6f, 0.6f, 0.6f);
	glPushMatrix();

	glTranslatef(p_x, 0.3f, p_z);
	glScalef(1.0f, 0.6f, 0.2f);
	glutSolidCube(1.0f);

	glPopMatrix();

	// cross on the tombstone
	glColor3f(0.7f, 0.7f, 0.7f);
	glPushMatrix();
	glTranslatef(p_x, 1.0f, p_z);

	// vertical arm
	glPushMatrix();
	glScalef(0.1f, 1.2f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// horizontal arm
	glPushMatrix();
	glScalef(0.6f, 0.1f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();
	glPopMatrix();
}

void DrawMausoleum(float p_x, float p_z) {
	glColor3f(0.5f, 0.5f, 0.5f);
	glPushMatrix();

	glTranslatef(p_x, 1.0f, p_z);
	glScalef(2.0f, 2.0f, 2.0f);
	glutSolidCube(1.0f);

	glPopMatrix();

	// door (a black quad)
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);

	glVertex3f(p_x - 0.5f, 0.0f, p_z + 1.01f);
	glVertex3f(p_x + 0.5f, 0.0f, p_z + 1.01f);
	glVertex3f(p_x + 0.5f, 1.5f, p_z + 1.01f);
	glVertex3f(p_x - 0.5f, 1.5f, p_z + 1.01f);

	glEnd();
}

void DrawLantern(float p_x, float p_z) {
	// post
	glColor3f(0.3f, 0.3f, 0.3f);
	glPushMatrix();

	glTranslatef(p_x, 1.0f, p_z);
	glScalef(0.1f, 2.0f, 0.1f);
	glutSolidCube(1.0f);

	glPopMatrix();

	// light
	glColor3f(1.0f, 0.9f, 0.6f);
	glPushMatrix();

	glTranslatef(p_x, 2.2f, p_z);
	glutSolidSphere(0.2f, 12, 12);

	glPopMatrix();
}

void DrawBush(float p_x, float p_z) {
	glColor3f(0.1f, 0.4f, 0.1f);
	glPushMatrix();

	glTranslatef(p_x, 0.5f, p_z);
	glScalef(1.5f, 1.0f, 1.5f);
	glutSolidSphere(1.0f, 12, 12);

	glPopMatrix();
}

void DrawGround() {
	// grass
	glColor3f(0.2f, 0.4f, 0.2f);
	glBegin(GL_QUADS);

	glVertex3f(-20, 0, -20); glVertex3f(20, 0, -20);
	glVertex3f(20, 0, 20); glVertex3f(-20, 0, 20);

	glEnd();
}

void DrawFence() {
	glColor3f(0.5f, 0.3f, 0.2f);
	for (int i = -15; i <= 15; i += 5) {
		// posts
		glPushMatrix();

		glTranslatef(i, 1.0f, -15);
		glScalef(0.2f, 2.0f, 0.2f);
		glutSolidCube(1.0f);

		glPopMatrix();
		glPushMatrix();

		glTranslatef(i, 1.0f, 15);
		glScalef(0.2f, 2.0f, 0.2f);
		glutSolidCube(1.0f);

		glPopMatrix();

		// crossbars
		glColor3f(0.6f, 0.4f, 0.3f);
		glPushMatrix();

		glTranslatef(i, 1.5f, -15);
		glScalef(5.0f, 0.1f, 0.1f);
		glutSolidCube(1.0f);

		glPopMatrix();
		glPushMatrix();

		glTranslatef(i, 1.5f, 15);
		glScalef(5.0f, 0.1f, 0.1f);
		glutSolidCube(1.0f);

		glPopMatrix();
	}
}
#pragma endregion

#pragma region Update
void MovementUpdate() {
	float dx = 0, dz = 0;

	// get input
	if (keys['w'] || keys['W']) {
		dx += sinf(yaw) * moveSpeed;
		dz += -cosf(yaw) * moveSpeed;
	}
	if (keys['s'] || keys['S']) {
		dx += -sinf(yaw) * moveSpeed;
		dz += cosf(yaw) * moveSpeed;
	}
	if (keys['d'] || keys['D']) { yaw += rotSpeed; }
	if (keys['a'] || keys['A']) { yaw -= rotSpeed; }

	float newX = px + dx;
	float newZ = pz + dz;
	if (!IsColliding(newX, newZ)) {
		px = newX;
		pz = newZ;
	}
}

void MinimapUpdate() {
	// toggle minimap
	if (!prevKeys['m'] && keys['m']) { minimapVisible = !minimapVisible; }
	if (!minimapVisible) { return; }

	// zoom in/out
	if (keys['-'] && minimapZoom < maxZoom) { minimapZoom += 0.1f; }
	else if (keys['+'] && minimapZoom > minZoom) { minimapZoom -= 0.1f; }
}

void PreviousKeysUpdate() {
	for (int i = 0; i < 256; i++) {
		if (keys[i] != prevKeys[i]) { prevKeys[i] = keys[i]; }
	}
}

void Update() {
	MovementUpdate();
	MinimapUpdate();

	PreviousKeysUpdate();

	glutPostRedisplay();
}
#pragma endregion

#pragma region Display

void DrawMinimap() {
	if (!minimapVisible) { return; }

	// setup 2D orthographic projection in minimap viewport
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();

	glLoadIdentity();
	glOrtho(-minimapZoom, minimapZoom, -minimapZoom, minimapZoom, -1, 1);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-px, -pz, 0.0f);

	// draw background
	glColor3f(0.1f, 0.1f, 0.1f);
	glBegin(GL_QUADS);

	glVertex2f(-15, -15); glVertex2f(15, -15);
	glVertex2f(15, 15);  glVertex2f(-15, 15);

	glEnd();

	// foreach tomb
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

	// mausoleum
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2f(8 - 1, 8 - 1);
	glVertex2f(8 + 1, 8 - 1);
	glVertex2f(8 + 1, 8 + 1);
	glVertex2f(8 - 1, 8 + 1);
	glEnd();

	// player indicator
	float dx = sinf(yaw) * 2, dz = -cosf(yaw) * 2;
	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(px, pz);
	glVertex2f(px + dx * 0.5f, pz + dz * 0.5f);
	glVertex2f(px - dz * 0.2f, pz + dx * 0.2f);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float dirX = sinf(yaw);
	float dirZ = -cosf(yaw);
	gluLookAt(px, py, pz,
		px + dirX, py, pz + dirZ,
		0.0f, 1.0f, 0.0f
	);

	GLfloat lightPos[] = { px, py, pz, 1.0f };
	GLfloat spotDir[] = { dirX, 0.0f, dirZ };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spotDir);

	// ground and fence
	DrawGround();
	DrawFence();

	// trails
	DrawPath(0, 0, 3, 20);
	DrawPath(-5, 5, 2, 10);

	// tombs and mausoleums
	DrawTomb(2, -2);
	DrawTomb(-3, 4);
	DrawMausoleum(8, 8);

	// lanterns and bushes
	DrawLantern(0, -8);
	DrawLantern(5, 5);
	DrawBush(-2, -7);

	// minimap viewport
	glViewport(winW - 200, winH - 200, 200, 200);
	DrawMinimap();

	// restore full viewport
	glViewport(0, 0, winW, winH);
	glutSwapBuffers();
}
#pragma endregion

void Reshape(int p_w, int p_h) {
	if (p_h == 0) { p_h = 1; }

	winW = p_w;
	winH = p_h;

	glViewport(0, 0, p_w, p_h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)p_w / (double)p_h, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
}

void KeyUp(unsigned char key, int x, int y) { keys[key] = false; }

void ObstaclesInit() {
	obstacles.clear();

	// Tombs (1x0.6 base centered on each position)
	for (auto& t : Tomb) {
		float halfW = 0.5f, halfD = 0.3f;
		obstacles.push_back({
			t.x - halfW, t.x + halfW,
			t.z - halfD, t.z + halfD
			});
	}

	// Mausoleums (2x2 base centered on each position)
	struct { float x, z; } maus[] = {
		{ 8.0f,  8.0f }
	};
	for (auto& m : maus) {
		float half = 1.0f;  // 2/2
		obstacles.push_back({
			m.x - half, m.x + half,
			m.z - half, m.z + half
			});
	}

	// Lanterns (0.1x0.1 pole)
	struct { float x, z; } lamps[] = {
		{  0.0f, -8.0f },
		{  5.0f,  5.0f }
	};
	for (auto& l : lamps) {
		float half = 0.05f;
		obstacles.push_back({
			l.x - half, l.x + half,
			l.z - half, l.z + half
			});
	}

	// Perimeter walls (thickness = 0.5 units)
	// South
	obstacles.push_back({
		-15.0f,   // minX
		 15.0f,   // maxX
		-15.25f,  // minZ
		-14.75f   // maxZ
		});
	// North
	obstacles.push_back({
		-15.0f,
		 15.0f,
		 14.75f,
		 15.25f
		});
	// West
	obstacles.push_back({
		-15.25f,  // minX
		-14.75f,  // maxX
		-15.0f,   // minZ
		 15.0f    // maxZ
		});
	// East
	obstacles.push_back({
		 14.75f,
		 15.25f,
		-15.0f,
		 15.0f
		});
}

void Timer(int value) {
	// Incrementa ángulo del sol (360° cada 30 segundos)
	sunAngle += 0.36f;
	if (sunAngle >= 360.0f) sunAngle -= 360.0f;

	float rad = sunAngle * PI / 180.0f;
	float intensity = sinf(rad);  // >0 es día, <=0 es noche
	if (intensity < 0.0f) intensity = 0.0f;

	// Luz solar (GL_LIGHT0)
	GLfloat sunAmbient[] = { 0.2f * intensity, 0.2f * intensity, 0.15f * intensity, 1.0f };
	GLfloat sunDiffuse[] = { 0.9f * intensity, 0.9f * intensity, 0.7f * intensity, 1.0f };
	GLfloat sunSpecular[] = { 1.0f * intensity, 1.0f * intensity, 0.8f * intensity, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);

	// Posición sol (en X-Y para simular movimiento vertical)
	const float radius = 20.0f;
	GLfloat sunPos[] = { radius * cos(rad), radius * sin(rad), 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);

	// Luz lunar (GL_LIGHT1), posición opuesta al sol
	float moonRad = rad + PI;
	GLfloat moonPos[] = { radius * cos(moonRad), radius * sin(moonRad), 0.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION, moonPos);

	// Cambio de color de fondo para simular cielo día/noche
	if (intensity <= 0.0f) {
		isNight = true;
		glClearColor(0.05f, 0.05f, 0.1f, 1.0f);  // color oscuro noche
	}
	else {
		isNight = false;
		glClearColor(0.5f, 0.8f, 1.0f, 1.0f);   // color azul claro día
	}

	// Control linterna: solo visible y activa si es de noche y el usuario la ha encendido
	if (isNight && flashlightOn) {
		glEnable(GL_LIGHT2);
		// Actualizar posición y dirección de la linterna (igual que la tienes ahora)
	}
	else {
		glDisable(GL_LIGHT2);
	}

	Update();

	glutPostRedisplay();
	glutTimerFunc(30, Timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(winW, winH);
	glutCreateWindow("Juan no paga la factura de la luz, muy mal Juan");

	glEnable(GL_DEPTH_TEST);

	InitLighting();

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutTimerFunc(30, Timer, 0);

	// keyboard input
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);

	ObstaclesInit();

	glutMainLoop();
	return 0;
}