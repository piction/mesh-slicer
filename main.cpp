
#include "log.h"
#include "commandLineParser.h"
#include "pointTransformer.h"


using namespace std;

int main(int argc, char* argv[])
{ 
    Log::Init();
    LOG_INFO("Stared slicer");

    // CommandLineParser parser;
    // if ( parser.tryParse( argc, argv) ) {
    //     LOG_DEBUG("Failed parsing command line parameters");
    //     return 1;
    // }

    // string objFile = parser.getObjFilePath();
    // string planeFile = parser.getPlaneDescriptionFilePath();

}