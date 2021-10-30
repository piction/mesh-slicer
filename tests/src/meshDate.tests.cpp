#include <gtest/gtest.h>
#include "log.h"

#include "meshData.h"

using namespace glm;

// check if no triangles are present no vertices will endup in the resulting file
TEST(MeshData,removeNotUsedNoData ){
    Log::Init();
   
    MeshData sut;
    for (int i = 0; i < 40; i++) {
        sut.vertices.push_back(vec3((float ) i , (float ) i, (float ) i));
    }
    
    sut.removeUnusedVertices();
    EXPECT_TRUE(sut.triangelVertices.size() == 0); // no triangles
    EXPECT_TRUE(sut.vertices.size() == 0); // no vertices 
}

// check that alle vertices that have no linked triangle
TEST(MeshData,removeNotUsed ){
    Log::Init();
   
    MeshData sut;
    for (int i = 0; i < 40; i++) {
        sut.vertices.push_back(vec3((float ) i , (float ) i, (float ) i));
    }
    for ( int j=10; j < 28; j+=3) {        
        sut.triangelVertices.push_back(TriangleVertices(j,j+1,j+2));
        sut.triangelVertices.push_back(TriangleVertices(j+2,j+1,j));
    }
    int lengthTrianglesBeforeRemove = sut.triangelVertices.size();
    
    sut.removeUnusedVertices();
    EXPECT_TRUE(sut.triangelVertices[0].one == 1); // first point of first triangle should always be 1
    EXPECT_TRUE(sut.triangelVertices[0].two == 2); // first point of first triangle should always be 2
    EXPECT_TRUE(sut.triangelVertices[0].three == 3); // first point of first triangle should always be 3
    EXPECT_TRUE(sut.vertices.size() == 18);
    EXPECT_TRUE(sut.triangelVertices.size() == lengthTrianglesBeforeRemove);

    // only vertices with content above or equal are used by the triangles, filter should get others out
    for ( auto &v: sut.vertices) {
        EXPECT_TRUE(v.x >= 9 && v.y >= 9 && v.z >= 9);
        EXPECT_TRUE(v.x <= 29 && v.y <= 29 && v.z <= 29);
    }

}


