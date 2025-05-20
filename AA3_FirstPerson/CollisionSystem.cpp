#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {
}

void CollisionSystem::Initialize() {
    ClearObstacles();

    // Tomb positions from original code
    struct { float x, z; } Tomb[] = {
        {  2.0f, -2.0f },
        { -3.0f,  4.0f }
    };

    // Add tombs
    for (auto& t : Tomb) {
        float halfW = 0.5f, halfD = 0.3f;
        AddObstacle(t.x - halfW, t.x + halfW, t.z - halfD, t.z + halfD);
    }

    // Add mausoleums
    struct { float x, z; } maus[] = {
        { 8.0f,  8.0f }
    };
    for (auto& m : maus) {
        float half = 1.0f;
        AddObstacle(m.x - half, m.x + half, m.z - half, m.z + half);
    }

    // Add lanterns
    struct { float x, z; } lamps[] = {
        {  0.0f, -8.0f },
        {  5.0f,  5.0f }
    };
    for (auto& l : lamps) {
        float half = 0.05f;
        AddObstacle(l.x - half, l.x + half, l.z - half, l.z + half);
    }

    // Add perimeter walls
    // South
    AddObstacle(-15.0f, 15.0f, -15.25f, -14.75f);
    // North
    AddObstacle(-15.0f, 15.0f, 14.75f, 15.25f);
    // West
    AddObstacle(-15.25f, -14.75f, -15.0f, 15.0f);
    // East
    AddObstacle(14.75f, 15.25f, -15.0f, 15.0f);
}

bool CollisionSystem::IsColliding(float objectX, float objectZ) const {
    for (const AABB& obst : obstacles) {
        if (objectX > obst.minX && objectX < obst.maxX &&
            objectZ > obst.minZ && objectZ < obst.maxZ) {
            return true;
        }
    }
    return false;
}

void CollisionSystem::AddObstacle(float minX, float maxX, float minZ, float maxZ) {
    obstacles.push_back({ minX, maxX, minZ, maxZ });
}

void CollisionSystem::ClearObstacles() {
    obstacles.clear();
}