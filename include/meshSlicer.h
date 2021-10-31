#pragma once

#include <memory>

#include <glm/glm.hpp>
#include "log.h"
#include "meshData.h"
#include "pointTransformer.h"
#include "utils.h"
#include "linePlaneIntersectionCache.h"


class MeshSlicer {
    public :
        MeshSlicer (const std::shared_ptr<MeshData> meshData) ;


        void slice(const glm::vec3 & planeOrigin ,const glm::vec3 & planeNormal, MeshData & resultUpper , MeshData & resultLower ) ;
    private:
        std::shared_ptr<MeshData> _meshData;
};