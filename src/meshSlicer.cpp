#include "meshSlicer.h"

MeshSlicer::MeshSlicer (const std::shared_ptr<MeshData> meshData) :_meshData(meshData) {}

void MeshSlicer::slice(const glm::vec3 & planeOrigin ,const glm::vec3 & planeNormal , MeshData & resultUpper , MeshData & resultLower ) {


    // create a transformer to reference mesh to the referenc of the cutting plane located on XY plane
    glm::vec3 vectorUp (0.0f,0.0f,1.0f);
    PointTransformer transformer( vectorUp,planeNormal,planeOrigin );
    
    // create a new mesh to calculate intersections
    MeshData transformedMesh = *_meshData; // copy originalMesh
    transformedMesh.transformMesh([& transformer ](const glm::vec3 & v)->glm::vec3 {return transformer.transformOriginToTarget(v);});

    // the transformed mesh is transfomed so that the cutting plane is aligned with the XY plane.
    // polygons above this plane should be copied to the upper-mesh and polygons below to the resultLower 
    // polygons that cross the XY plane are split in two
    // The upper mesh is used to add all new vertices. not used vertices are cleaned aftwards
    resultUpper.vertices = _meshData->vertices;    

    // Due connecting polygons, polygon lines between are part of multiple polygons,
    // use a cache to refer to the same intersections and don't recalculate it resulting in duplicated vertices 
    LinePlaneIntersectionCache lineIntersectionsCache;


    // evalute each polygon
    for ( auto & f: transformedMesh.triangelVertices) {   
        
        // order indices of polygon from top to bottom regarding z value
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
        // verify the ordering
        if(transformedMesh.vertices[top].z >= transformedMesh.vertices[middle].z  &&  transformedMesh.vertices[middle].z >= transformedMesh.vertices[bottom].z ) {
            // LOG_DEBUG("OK")
        } else {
            LOG_ERROR("Invalid top/middle/bottom");
        }

        // check if a split is needed 
        if ( transformedMesh.vertices[bottom].z >= 0) {        
            resultUpper.triangelVertices.push_back(f);
        }  else if ( transformedMesh.vertices[top].z <= 0) {        
            resultLower.triangelVertices.push_back(f);
        } else {
            
            // SPLITING POLYGON
            glm::vec3 & transTopVertic = transformedMesh.vertices[top];
            glm::vec3 & transMiddleVertic = transformedMesh.vertices[middle];
            glm::vec3 & transBottomVertic = transformedMesh.vertices[bottom]; 

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
    resultUpper.removeUnusedVertices();
    resultLower.removeUnusedVertices();
  

}