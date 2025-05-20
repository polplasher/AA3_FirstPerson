#include "Scene.h"
#include "CollisionSystem.h"

Scene::Scene() {
}

void Scene::Initialize(CollisionSystem* collisionSystem) {
    // Initialize scene objects and add them to the collision system
    // This would replace the hard-coded objects in the original code

    // For simplicity, we can add objects with their positions here
    // In a real implementation, these might be loaded from a file

    // Add tombs
    objects.push_back({ SceneObject::TOMB, 2.0f, -2.0f, 1.0f, 1.0f, 0.5f });
    objects.push_back({ SceneObject::TOMB, -3.0f, 4.0f, 1.0f, 1.0f, 0.5f });

    // Add mausoleum
    objects.push_back({ SceneObject::MAUSOLEUM, 8.0f, 8.0f, 2.0f, 2.0f, 2.0f });

    // Add lanterns
    objects.push_back({ SceneObject::LANTERN, 0.0f, -8.0f, 0.1f, 2.0f, 0.1f });
    objects.push_back({ SceneObject::LANTERN, 5.0f, 5.0f, 0.1f, 2.0f, 0.1f });

    // Add bushes
    objects.push_back({ SceneObject::BUSH, -2.0f, -7.0f, 1.5f, 1.0f, 1.5f });

    // Add paths
    objects.push_back({ SceneObject::PATH, 0.0f, 0.0f, 3.0f, 0.1f, 20.0f });
    objects.push_back({ SceneObject::PATH, -5.0f, 5.0f, 2.0f, 0.1f, 10.0f });
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
        case SceneObject::MAUSOLEUM:
            DrawMausoleum(obj.x, obj.z);
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
        }
    }
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

void Scene::DrawMausoleum(float x, float z) {
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(x, 1.0f, z);
    glScalef(2.0f, 2.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Door (a black quad)
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex3f(x - 0.5f, 0.0f, z + 1.01f);
    glVertex3f(x + 0.5f, 0.0f, z + 1.01f);
    glVertex3f(x + 0.5f, 1.5f, z + 1.01f);
    glVertex3f(x - 0.5f, 1.5f, z + 1.01f);
    glEnd();
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