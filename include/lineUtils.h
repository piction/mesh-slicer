#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>
#include <cmath> 

#include "meshData.h"
#include "pointTransformer.h"


class LineUtils {
    public:
    static glm::vec3 getIntersectionWithXYplane(const glm::vec3 lineEnd, const glm::vec3 lineStart) {
        auto delta = lineEnd-lineStart;
        
        double xCross = abs(delta.x) < 1e-12 ? lineEnd.x : (((delta.z / delta.x * lineStart.x) - lineStart.z ) / (delta.z/delta.x));
        double yCross = abs(delta.y) < 1e-12 ? lineEnd.y : (((delta.z / delta.y * lineStart.y) - lineStart.z ) / (delta.z/delta.y));
        return glm::vec3(xCross,yCross,0);
    }
};