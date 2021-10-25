
#include "log.h"
#include "commandLineParser.h"
#include "pointTransformer.h"
#include "wavefrontObjSerializer.h"
#include "planeDescriptionSerializer.h"


using namespace std;

int main(int argc, char* argv[])
{ 
    Log::Init();
    LOG_INFO("Stared slicer");

    
    CommandLineParser parser {};
    if (! parser.tryParse( argc, argv) ) {
        LOG_DEBUG("Failed parsing command line parameters");
        return 1;
    }

    string objFile = parser.getObjFilePath();
    string planeFile = parser.getPlaneDescriptionFilePath();


    MeshData originalMesh = WavefrontObjSerializer::deserialize(objFile.c_str());
    glm::vec3 planeOrigin, planeNormal;
    PlaneDescriptionSerializer::deserialize(planeFile.c_str(), planeOrigin, planeNormal);
    
    LOG_DEBUG( "Original Mesh => vertices: " + std::to_string(originalMesh.vertices.size()) + ", triangles: " +std::to_string(originalMesh.triangelVertices.size())) ;
    LOG_DEBUG( "Plane=> origin:[" + glm::to_string(planeOrigin) + "], normal:[" + glm::to_string(planeNormal) + "]");
}