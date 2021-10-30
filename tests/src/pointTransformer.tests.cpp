#include <gtest/gtest.h>



#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>        // std::abs

#include "log.h"
#include "pointTransformer.h" 


using namespace glm;



TEST(point_transformer,basics ){
    Log::Init();
    LOG_INFO("test transformation");


	glm::vec3 vec1(+1.0f,  0.0f,  0.0f);
	glm::vec3 vec2( 0.0f, +1.0f,  0.0f);
    glm::vec3 vecToTransform( 5.0f, +0.0f,  0.0f);
    glm::vec3 trans( 10.0f, +10.0f,  10.0f);

    PointTransformer transformer (vec1, vec2,trans);
    
    glm::vec3 transformedToTarget  = transformer.transformOriginToTarget(vecToTransform);
    glm::vec3 transformedToBack  = transformer.transformTargetToOrigin(transformedToTarget);
    LOG_INFO("my vec" + glm::to_string(vecToTransform));
    LOG_INFO("my tranformed vector" + glm::to_string(transformedToTarget));
    LOG_INFO("my oringal vector" + glm::to_string(transformedToBack));

    EXPECT_TRUE ( std::abs(glm::distance(vecToTransform,transformedToBack)) < 1e6) << "difference after rotating back shuold be minimal";
}

