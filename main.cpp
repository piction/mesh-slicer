
#include "log.h"
#include "commandLineParser.h"
#include "pointTransformer.h"
#include "wavefrontObjSerializer.h"
#include "planeDescriptionSerializer.h"
#include "meshCreator.h"
#include "utils.h"
#include "linePlaneIntersectionCache.h"
#include "meshSlicer.h"

#include <map>
#include <memory>
#include <glm/glm.hpp>



using namespace std;

int main(int argc, char* argv[])
{ 
    Log::Init();
    LOG_INFO("Stared slicer");
    bool  shouldCreateDebugCuttingPlane=true;
    
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

    
   
    
    // create a mesh of the cutting plane as an easy validator 
    if ( shouldCreateDebugCuttingPlane) {   
        // create a transformer to reference mesh to the referenc of the cutting plane located on XY plane
         glm::vec3 vectorUp (0.0f,0.0f,1.0f);
        PointTransformer transformer( vectorUp,planeNormal,planeOrigin ); 
        MeshData cuttingPlaneMesh = MeshCreator::createMeshOfPlane(15,planeOrigin,planeNormal);
        WavefrontObjSerializer::serialize("planeToCut.obj", cuttingPlaneMesh);        
    }

    MeshData resultUpper, resultLower;
    MeshSlicer slicer (std::make_shared<MeshData>(originalMesh));
    slicer.slice(planeOrigin,planeNormal,resultUpper,resultLower);

    WavefrontObjSerializer::serialize("cuttedMesh-upper.obj", resultUpper);
    WavefrontObjSerializer::serialize("cuttedMesh-lower.obj", resultLower);


}