#include "CollisionSystem.h"
#include "Game.h"
#include "Scene.h"

Scene::Scene() {
}

Scene::~Scene() {
	// Clean up loaded models
	for (auto& pair : modelCache) { delete pair.second; }
	modelCache.clear();
}

void Scene::Initialize(CollisionSystem* collisionSystem) {
	// Add tombs
	objects.push_back({ SceneObject::TOMB, 2.0f, -2.0f, 1.0f, 1.0f, 0.5f });
	objects.push_back({ SceneObject::TOMB, -3.0f, 4.0f, 1.0f, 1.0f, 0.5f });

	// Add lanterns
	objects.push_back({ SceneObject::LANTERN, 0.0f, -8.0f, 0.1f, 2.0f, 0.1f });
	objects.push_back({ SceneObject::LANTERN, 5.0f, 5.0f, 0.1f, 2.0f, 0.1f });

	// Add bushes
	objects.push_back({ SceneObject::BUSH, -2.0f, -7.0f, 1.5f, 1.0f, 1.5f });

	// Add paths
	objects.push_back({ SceneObject::PATH, 0.0f, 0.0f, 3.0f, 0.1f, 20.0f });
	objects.push_back({ SceneObject::PATH, -5.0f, 5.0f, 2.0f, 0.1f, 10.0f });

	AddModel("models/mausoleum.obj", 7, 7, 1, -90);
	AddModel("models/gravestone1.obj", 4, -1);
	AddModel("models/gravestone2.obj", -5, -2, 1.4);

	// Add to collision system
	for (const SceneObject& obj : objects) {
		if (obj.type == SceneObject::MODEL) {
			float halfWidth = obj.width / 2.0f;
			float halfDepth = obj.depth / 2.0f;
			collisionSystem->AddObstacle(
				obj.x - halfWidth, obj.x + halfWidth,
				obj.z - halfDepth, obj.z + halfDepth
			);
		}
	}

	// Initialize lighting for lanterns
	int nextLightID = GL_LIGHT3; // Start from GL_LIGHT3 to avoid conflicts with sun/moon lights and flashlight
	for (SceneObject& obj : objects) {
		if (obj.type == SceneObject::LANTERN) {
			obj.lightID = nextLightID++;
			glEnable(obj.lightID);
			GLfloat lightPos[] = { obj.x, 2.2f, obj.z, 1.0f };
			GLfloat diffuse[] = { 1.0f, 0.8f, 0.2f, 1.0f };

			glLightfv(obj.lightID, GL_POSITION, lightPos);
			glLightfv(obj.lightID, GL_DIFFUSE, diffuse);
			glDisable(obj.lightID); // start off
		}
	}
}

void Scene::AddModel(const std::string& modelPath, float x, float z, float scale,
	float rotY, float r, float g, float b) {
	// Check if model is already loaded
	Model* model = nullptr;
	if (modelCache.find(modelPath) == modelCache.end()) {
		// Load model
		model = new Model();
		if (model->LoadModel(modelPath)) {
			model->SetColor(r, g, b);
			modelCache[modelPath] = model;
		}
		else {
			delete model;
			return; // Failed to load
		}
	}
	else {
		model = modelCache[modelPath];
	}

	// Add to scene objects
	SceneObject obj;
	obj.type = SceneObject::MODEL;
	obj.x = x;
	obj.z = z;
	obj.width = scale;
	obj.height = scale;
	obj.depth = scale;
	obj.rotationY = rotY;
	obj.model = model;
	obj.modelPath = modelPath;

	objects.push_back(obj);
}

void Scene::Draw() {
	// Draw ground and fence (static elements)
	DrawGround();
	DrawFence();

	// Draw scene objects
	for (const auto& obj : objects) {
		switch (obj.type) {
		case SceneObject::TOMB:
			DrawTomb(obj.x, obj.z);
			break;
		case SceneObject::LANTERN:
			DrawLantern(obj.x, obj.z);
			break;
		case SceneObject::BUSH:
			DrawBush(obj.x, obj.z);
			break;
		case SceneObject::PATH:
			DrawPath(obj.x, obj.z, obj.width, obj.depth);
			break;
		case SceneObject::MODEL:
			DrawModel(obj);
			break;
		}
	}
}

void Scene::DrawModel(const SceneObject& obj) {
	if (!obj.model) return;

	glPushMatrix();
	glTranslatef(obj.x, 0.0f, obj.z);
	glRotatef(obj.rotationY, 0.0f, 1.0f, 0.0f);
	glScalef(obj.width, obj.height, obj.depth);

	obj.model->Draw();

	glPopMatrix();
}

void Scene::DrawGround() {
	// Draw grass
	glColor3f(0.2f, 0.4f, 0.2f);
	glBegin(GL_QUADS);
	glVertex3f(-20, 0, -20);
	glVertex3f(20, 0, -20);
	glVertex3f(20, 0, 20);
	glVertex3f(-20, 0, 20);
	glEnd();
}

void Scene::DrawFence() {
	glColor3f(0.5f, 0.3f, 0.2f);
	for (int i = -15; i <= 15; i += 5) {
		// Posts
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

		// Crossbars
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

void Scene::DrawPath(float x, float z, float width, float length) {
	glColor3f(0.3f, 0.3f, 0.3f);
	glBegin(GL_QUADS);
	glVertex3f(x - width / 2, 0.01f, z - length / 2);
	glVertex3f(x + width / 2, 0.01f, z - length / 2);
	glVertex3f(x + width / 2, 0.01f, z + length / 2);
	glVertex3f(x - width / 2, 0.01f, z + length / 2);
	glEnd();
}

void Scene::DrawTomb(float x, float z) {
	// Base tombstone
	glColor3f(0.6f, 0.6f, 0.6f);
	glPushMatrix();
	glTranslatef(x, 0.3f, z);
	glScalef(1.0f, 0.6f, 0.2f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Cross on the tombstone
	glColor3f(0.7f, 0.7f, 0.7f);
	glPushMatrix();
	glTranslatef(x, 1.0f, z);

	// Vertical arm
	glPushMatrix();
	glScalef(0.1f, 1.2f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Horizontal arm
	glPushMatrix();
	glScalef(0.6f, 0.1f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	glPopMatrix();
}

void Scene::DrawLantern(float x, float z) {
	// Post
	glColor3f(0.3f, 0.3f, 0.3f);
	glPushMatrix();
	glTranslatef(x, 1.0f, z);
	glScalef(0.1f, 2.0f, 0.1f);
	glutSolidCube(1.0f);
	glPopMatrix();

	// Light
	glColor3f(1.0f, 0.9f, 0.6f);
	glPushMatrix();
	glTranslatef(x, 2.2f, z);
	glutSolidSphere(0.2f, 12, 12);
	glPopMatrix();
}

void Scene::DrawBush(float x, float z) {
	glColor3f(0.1f, 0.4f, 0.1f);
	glPushMatrix();
	glTranslatef(x, 0.5f, z);
	glScalef(1.5f, 1.0f, 1.5f);
	glutSolidSphere(1.0f, 12, 12);
	glPopMatrix();
}