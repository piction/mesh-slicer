
#include "log.h"
#include "commandLineParser.h"
#include "pointTransformer.h"
#include "wavefrontObjSerializer.h"
#include "planeDescriptionSerializer.h"
#include "meshCreator.h"
#include "lineUtils.h"
#include "linePlaneIntersectionCache.h"

#include <map>
#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>



using namespace std;

int main(int argc, char* argv[])
{ 
    Log::Init();
    LOG_INFO("Stared slicer");

    
    CommandLineParser parser {};
    if (! parser.tryParse( argc, argv) ) {
        LOG_WARNING("Failed parsing command line parameters");
        return 1;
    }

    string objFile = parser.getObjFilePath();
    string planeFile = parser.getPlaneDescriptionFilePath();


    MeshData originalMesh = WavefrontObjSerializer::deserialize(objFile.c_str());
    glm::vec3 planeOrigin, planeNormal;
    PlaneDescriptionSerializer::deserialize(planeFile.c_str(), planeOrigin, planeNormal);
    
    LOG_DEBUG( "Original Mesh => vertices: " + std::to_string(originalMesh.vertices.size()) + ", triangles: " +std::to_string(originalMesh.triangelVertices.size())) ;
    LOG_DEBUG( "Plane=> origin:[" + glm::to_string(planeOrigin) + "], normal:[" + glm::to_string(planeNormal) + "]");

    // transform mesh to plane origin
    glm::vec3 vectorUp (0.0f,0.0f,1.0f);
    MeshData transformedMesh;
    PointTransformer transformer( vectorUp,planeNormal,planeOrigin );
    
    for ( auto & v : originalMesh.vertices ) {
        transformedMesh.vertices.push_back(transformer.transformOriginToTarget(v));
    }
    for ( auto & f: originalMesh.triangelVertices) {
        transformedMesh.triangelVertices.push_back(f);
    }

    WavefrontObjSerializer::serialize("transformedMesh.obj", transformedMesh);
    MeshData cuttingPlaneMesh = MeshCreator::createMeshOfPlane(15,planeOrigin,planeNormal);
    MeshData cuttingPlaneTransformed ;
    
    for ( auto & v : cuttingPlaneMesh.vertices ) {
        cuttingPlaneTransformed.vertices.push_back(transformer.transformOriginToTarget(v));
    }
    for ( auto & f: cuttingPlaneMesh.triangelVertices) {
        cuttingPlaneTransformed.triangelVertices.push_back(f);
    }

    WavefrontObjSerializer::serialize("planeToCut.obj", cuttingPlaneMesh);
    WavefrontObjSerializer::serialize("planeToCut-transformed.obj", cuttingPlaneTransformed);

    MeshData resultUpper, resultLower ;
    
    resultUpper.vertices = originalMesh.vertices;    
    LinePlaneIntersectionCache lineIntersectionsCache;

    for ( auto & f: transformedMesh.triangelVertices) {   
        int top = f.one -1 ;
        int middle = f.one -1 ;
        int bottom =f.one -1 ;
        
        if (transformedMesh.vertices[f.one-1].z >= transformedMesh.vertices[f.two-1].z ) {
            if (transformedMesh.vertices[f.two-1].z >=  transformedMesh.vertices[f.three-1].z) {
                top = f.one -1 ; middle = f.two-1 ; bottom = f.three-1; 
            } else if (transformedMesh.vertices[f.one-1].z >=  transformedMesh.vertices[f.three-1].z) {
                top = f.one -1 ; middle = f.three-1 ; bottom = f.two-1; 
            } else {
                top = f.three -1 ; middle = f.one-1 ; bottom = f.two-1; 
            }
        } else {
            if (transformedMesh.vertices[f.one-1].z >=  transformedMesh.vertices[f.three-1].z) {
                top = f.two -1 ; middle = f.one-1 ; bottom = f.three-1; 
            } else if (transformedMesh.vertices[f.two-1].z >=  transformedMesh.vertices[f.three-1].z) {
                top = f.two -1 ; middle = f.three-1 ; bottom = f.one-1; 
            } else {
                top = f.three -1 ; middle = f.two-1 ; bottom = f.one-1; 
            }
        }
        if(transformedMesh.vertices[top].z >= transformedMesh.vertices[middle].z  &&  transformedMesh.vertices[middle].z >= transformedMesh.vertices[bottom].z ) {
            // LOG_DEBUG("OK")
        } else {
            LOG_ERROR("Invalid top/middle/bottom");
        }

        if ( transformedMesh.vertices[bottom].z >= 0) {        
            resultUpper.triangelVertices.push_back(f);
        }  else if ( transformedMesh.vertices[top].z <= 0) {        
            resultLower.triangelVertices.push_back(f);
        } else {
            
            glm::vec3 transTopVertic = transformedMesh.vertices[top];
            glm::vec3 transMiddleVertic = transformedMesh.vertices[middle];
            glm::vec3 transBottomVertic = transformedMesh.vertices[bottom]; 

            glm::vec3 intersectMiddleToPlane,intersectTopToPlane;
            int indexTopIntersect = -1;
            int indexMiddleIntersect = -1;

             if (!lineIntersectionsCache.tryRetrieveFromCache(top,bottom,indexTopIntersect)) {
                    intersectTopToPlane = transformer.transformTargetToOrigin(
                        LineUtils::getIntersectionWithXYplane(transTopVertic,transBottomVertic));
                    resultUpper.vertices.push_back(intersectTopToPlane);
                    indexTopIntersect = resultUpper.vertices.size();
                    lineIntersectionsCache.addToCache(top,bottom,indexTopIntersect);
                }

            if (transformedMesh.vertices[middle].z >= 0) { // two on top
                
                if (!lineIntersectionsCache.tryRetrieveFromCache(middle,bottom,indexMiddleIntersect)) {
                    intersectMiddleToPlane= transformer.transformTargetToOrigin(
                        LineUtils::getIntersectionWithXYplane(transMiddleVertic,transBottomVertic));                    
                    resultUpper.vertices.push_back(intersectMiddleToPlane);
                    indexMiddleIntersect = resultUpper.vertices.size();
                    lineIntersectionsCache.addToCache(middle,bottom,indexMiddleIntersect);
                }        
                
                TriangleVertices newTriangleBottom ( indexTopIntersect, indexMiddleIntersect, bottom + 1);
                resultLower.triangelVertices.push_back(newTriangleBottom);

                resultUpper.triangelVertices.push_back( TriangleVertices ( indexTopIntersect, middle+1,top+1));
                resultUpper.triangelVertices.push_back( TriangleVertices ( indexTopIntersect, indexMiddleIntersect ,middle + 1));
 
            } else {
                // two below , one above

                if (!lineIntersectionsCache.tryRetrieveFromCache(top,middle,indexMiddleIntersect)) {
                     intersectMiddleToPlane = transformer.transformTargetToOrigin(
                        LineUtils::getIntersectionWithXYplane(transTopVertic,transMiddleVertic));
                    resultUpper.vertices.push_back(intersectMiddleToPlane);
                    indexMiddleIntersect = resultUpper.vertices.size();
                    lineIntersectionsCache.addToCache(top,middle,indexMiddleIntersect);
                }              
                

                TriangleVertices newTriangleBottom (  indexMiddleIntersect, indexTopIntersect , top + 1);
                resultUpper.triangelVertices.push_back(newTriangleBottom);

                resultLower.triangelVertices.push_back( TriangleVertices( indexTopIntersect, middle+1,bottom + 1));
                resultLower.triangelVertices.push_back( TriangleVertices ( indexMiddleIntersect, indexTopIntersect  ,middle + 1));
            }
        
        }
    }

    resultLower.vertices = resultUpper.vertices;    
    // resultUpper.removeUnusedVertices();
    // resultLower.removeUnusedVertices();
    WavefrontObjSerializer::serialize("cuttedMesh-upper.obj", resultUpper);
    WavefrontObjSerializer::serialize("cuttedMesh-lower.obj", resultLower);





}