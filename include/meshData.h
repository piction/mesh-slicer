#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>

struct TriangleVertices {
    int one;
    int two;
    int three;
    TriangleVertices(){};
    TriangleVertices ( int one,int two, int three ) : one(one), two(two), three(three){};
};

struct MeshData {
    std::vector<glm::vec3> vertices;
    std::vector<TriangleVertices> triangelVertices;
};