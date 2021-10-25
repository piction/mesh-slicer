#include <gtest/gtest.h>



#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>        // std::abs

#include "log.h"
#include "meshCreator.h" 
#include "wavefrontObjSerializer.h"


using namespace glm;



TEST(meshCreatro,createPlane ){
    Log::Init();
    LOG_INFO("test transformation");


	glm::vec3 originOfPlane(+0.015f,  0.0f,  0.021f);
	glm::vec3 normalOnPlane( 0.60f, +0.45f,  0.30f);

    auto planeMesh = MeshCreator::CreateMeshOfPlane(0.3,originOfPlane,normalOnPlane);

    char const *fileName = "testData/planeMesh.obj";
     
    WavefrontObjSerializer::Serialize(fileName,planeMesh);

}