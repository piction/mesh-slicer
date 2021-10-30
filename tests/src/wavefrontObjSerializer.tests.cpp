#include <gtest/gtest.h>

#include "wavefrontObjSerializer.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <cmath>        // std::abs

#include "log.h"
#include "pointTransformer.h" 




TEST(wavefrontObjSerializer,DesiredSerialization ){
    Log::Init();
    LOG_INFO("DesiredSerialization OBJ");
    
    char const *fileName = "testData/bunny.obj";
    auto result = WavefrontObjSerializer::deserialize(fileName);

    EXPECT_TRUE(result.vertices.size() > 10);
    EXPECT_TRUE(result.triangelVertices.size() > 10);

    LOG_DEBUG("Number of vertices found: " + std::to_string(result.vertices.size()) );
}

TEST(wavefrontObjSerializer,Serialize ){
    Log::Init();
    LOG_INFO("Serialize mesh to OBJ");
    char const *fileName = "testData/serialize-output.obj";
    MeshData data;
    
    for ( int i = 0; i < 10; i++) {
        data.vertices.push_back(glm::vec3(+10.0f,  1.0f, (float) i));
        data.vertices.push_back(glm::vec3(+15.0f,  12.0f,  (float) i));
        data.vertices.push_back(glm::vec3(+20.0f,  1.0f,(float) i));
        TriangleVertices tr;
        tr.one =(int) (i*3)+ 1;
        tr.two = (int) (i*3)+2;
        tr.three = (i*3)+3;

        data.triangelVertices.push_back(tr);
    }
    
    WavefrontObjSerializer::serialize(fileName,data);
}
TEST(wavefrontObjSerializer,SerializeNoData ){
    Log::Init();
    LOG_INFO("Serialize mesh to OBJ");
    char const *fileName = "testData/serialize-no-data-output.obj";
    MeshData data;        
    WavefrontObjSerializer::serialize(fileName,data);
}


TEST(wavefrontObjSerializer,DesiredSerializationTransform ){
    Log::Init();
    LOG_INFO("DesiredSerialization OBJ");
    
    char const *fileName = "testData/bunny.obj";
    char const *fileNameCopy = "testData/bunny-copy.obj";
    auto result = WavefrontObjSerializer::deserialize(fileName);

	glm::vec3 vec1(+1.0f,  0.0f,  0.0f);
	glm::vec3 vec2( 0.0f, +1.0f,  0.0f);
    glm::vec3 vecToTransform( 5.0f, +0.0f,  0.0f);
    glm::vec3 trans( 0.10f, +0.10f,  0.50f);

    PointTransformer transformer (vec1, vec2,trans);
    
    glm::vec3 transformedToTarget  = transformer.transformOriginToTarget(vecToTransform);
    glm::vec3 transformedToBack  = transformer.transformTargetToOrigin(transformedToTarget);
    LOG_INFO("my vec" + glm::to_string(vecToTransform));
    LOG_INFO("my tranformed vector" + glm::to_string(transformedToTarget));
    LOG_INFO("my oringal vector" + glm::to_string(transformedToBack));

    std::vector<glm::vec3> transformedList;
    for ( auto & v : result.vertices) {
        glm::vec3 transformedToTarget  = transformer.transformOriginToTarget(v);
        transformedList.push_back(transformedToTarget);
    }
    result.vertices =transformedList;
     WavefrontObjSerializer::serialize (fileNameCopy, result);
}



TEST(wavefrontObjSerializer,cleanUpMeshAndSave ){
    Log::Init();
    LOG_INFO("DesiredSerialization OBJ");
    
    char const *fileName = "testData/half-bunny-duplicates.obj";
    char const *fileNameOut = "testData/bunny-duplicates-output.obj";
    auto result = WavefrontObjSerializer::deserialize(fileName);
    result.removeUnusedVertices();
     WavefrontObjSerializer::serialize (fileNameOut, result);
}
