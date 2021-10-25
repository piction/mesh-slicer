#include <gtest/gtest.h>



#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "log.h"


#include "planeDescriptionSerializer.h"

// {
//     "origin":[0.015,0.0,0.021] , "normal":[0.60,0.450,0.30]
// }



TEST(PlaneDescriptionSerializer,basics ){
    Log::Init();
    
    glm::vec3 origin;
    glm::vec3 normal;

    PlaneDescriptionSerializer::deserialize("testData/plane-description.json", origin, normal);

    LOG_DEBUG("origin PARSED:" + glm::to_string(origin));
    LOG_DEBUG("normal PARSED:" + glm::to_string(normal));

    bool isTrue = true;
    EXPECT_TRUE(isTrue) << "dummy test!!";
}


