#include <gtest/gtest.h>
#include "log.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

TEST(empty,basics ){
    Log::Init();
    LOG_INFO("empty test ");
    bool isTrue = true;
    EXPECT_TRUE(isTrue) << "dummy test!!";
}

TEST(empty,testGlm ){
    Log::Init();
    LOG_INFO("test GLM");


	glm::vec3 Xpos(+1.0f,  0.0f,  0.0f);
	glm::vec3 Ypos( 0.0f, +1.0f,  0.0f);
	glm::vec3 Zpos( 0.0f,  0.0f, +1.0f);
	glm::vec3 Xneg(-1.0f,  0.0f,  0.0f);
	glm::vec3 Yneg( 0.0f, -1.0f,  0.0f);
	glm::vec3 Zneg( 0.0f,  0.0f, -1.0f);
	

    quat MyQuaternion;

    // Direct specification of the 4 components
    // You almost never use this directly
    //MyQuaternion = quat(w,x,y,z); 

    // Conversion from Euler angles (in radians) to Quaternion
    vec3 EulerAngles(90, 45, 0);
    MyQuaternion = quat(EulerAngles);
    LOG_INFO("test " + glm::to_string(MyQuaternion));

    bool isTrue = true;
    EXPECT_TRUE(isTrue) << "dummy test!!";
}

