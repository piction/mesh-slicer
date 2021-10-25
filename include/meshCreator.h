#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>

#include <vector>

#include "meshData.h"
#include "pointTransformer.h"


class MeshCreator {
    public:
    static MeshData CreateMeshOfPlane  (double squareSizeOfPlane , glm::vec3 originPlane , glm::vec3 normalOfPlane) {
        glm::vec3 originUp = glm::vec3(1.0f,0.0f, 0.0f);
        
        PointTransformer transformer(originUp,normalOfPlane,originPlane);

        // create square 
        glm::vec3 leftTop = glm::vec3(-squareSizeOfPlane/2,+squareSizeOfPlane/2,0.0f);
        glm::vec3 rightTop = glm::vec3(+squareSizeOfPlane/2,+squareSizeOfPlane/2,0.0f);
        glm::vec3 rightBottom = glm::vec3(+squareSizeOfPlane/2,-squareSizeOfPlane/2,0.0f);
        glm::vec3 leftBottom = glm::vec3(-squareSizeOfPlane/2,-squareSizeOfPlane/2,0.0f);

        MeshData data;
        data.vertices.push_back(transformer.TransformTargetToOrigin(leftTop));  // index 1
        data.vertices.push_back(transformer.TransformTargetToOrigin(rightTop)); // index 2
        data.vertices.push_back(transformer.TransformTargetToOrigin(rightBottom)); // index 3
        data.vertices.push_back(transformer.TransformTargetToOrigin(leftBottom)); // index 4

        data.triangelVertices.push_back(TriangleVertices(1,2,4));
        data.triangelVertices.push_back(TriangleVertices(4,3,2));
        
        return data;
    }

};