#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>
#include <map>
#include <functional>
#include <exception>
#include "log.h"

struct TriangleVertices {
    int one;
    int two;
    int three;    
    TriangleVertices(){};
    TriangleVertices ( int one,int two, int three ) : one(one), two(two), three(three){};
};

class MeshData {
    public:
        std::vector<glm::vec3> vertices;
        std::vector<TriangleVertices> triangelVertices;

        void removeUnusedVertices ();
        void transformMesh(std::function<glm::vec3 (glm::vec3 &)> transformVerticeFn);
        
};