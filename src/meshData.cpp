#include "meshData.h"
#include <map>
#include <functional>


using namespace glm;

void MeshData::removeUnusedVertices() {
    
    int verticesCounter=1;
    std::vector<int> lookup(vertices.size(),-1);
    std::vector<vec3> filteredVertices;
     
    auto renameFnc = [&](int trianglePointIndx)-> int{
        int newIndx;
        if (lookup[trianglePointIndx] < 0) {
            lookup[trianglePointIndx] = verticesCounter;
            filteredVertices.push_back(vertices[trianglePointIndx-1]);        
            newIndx =verticesCounter; 
            verticesCounter++;   
        } else {
            newIndx= lookup[trianglePointIndx];
        }
        return newIndx;
    };    

    for (auto & v : triangelVertices) {        
        v.one = renameFnc(v.one);
        v.two = renameFnc(v.two);
        v.three = renameFnc(v.three);
    }
    vertices = filteredVertices;
}