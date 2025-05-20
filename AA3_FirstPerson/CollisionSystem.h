#pragma once
#include <vector>

struct AABB {
    float minX, maxX, minZ, maxZ;
};

class CollisionSystem {
private:
    std::vector<AABB> obstacles;

public:
    CollisionSystem();

    void Initialize();
    bool IsColliding(float objectX, float objectZ) const;

    void AddObstacle(float minX, float maxX, float minZ, float maxZ);
    void ClearObstacles();
};