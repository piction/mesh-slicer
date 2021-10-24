#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>


class PointTransformer {
    public : 
        PointTransformer (glm::vec3 orgin, glm::vec3 target,glm::vec3 offsetToOrigin );
    
    public :
        // Returns a quaternion such that q*start = dest
        glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

        glm::vec3 TransformOriginToTarget(glm::vec3 v);
        glm::vec3 TransformTargetToOrigin(glm::vec3 v);

        // use boolean to lazy calculate to rotations
        glm::quat _rotateOriginToTarget;
        bool _rotateOriginToTargetIsSet;
        glm::quat _rotateTargetToOrigin;
        bool _rotateTargetToOriginIsSet;


        glm::vec3 _origin; 
        glm::vec3 _target;
        glm::vec3 _offsetToOrigin;



};