#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include "log.h"
#include "linePlaneIntersectionCache.h"


TEST(linePlaneIntersectionCache,basics ){
    Log::Init();
    
    LinePlaneIntersectionCache sut;
    int intersectionIndexFromCache;
    // cannot retrieve from empty
    EXPECT_FALSE(sut.tryRetrieveFromCache(1,2,intersectionIndexFromCache)) << "Should not be able to retrieve from empty cache";
    
    // cannot add same index for top and bottom
    bool hasThrown = false;
    try
    {
        sut.addToCache(1,1,11);
    }
    catch(...)
    {
        hasThrown = true;
    }
    EXPECT_TRUE(hasThrown)<< "Should throw if index bottom and index top are the same";

    sut.addToCache(1,0,11);
    sut.addToCache(1,0,11); // twice should not be any problem
    sut.addToCache(1,2,11);
    sut.addToCache(1,3,13);
    sut.addToCache(2,3,23);

    
    EXPECT_FALSE(sut.tryRetrieveFromCache(4,10,intersectionIndexFromCache)) << "should not find this entry in the cache";
    EXPECT_TRUE(sut.tryRetrieveFromCache(1,3,intersectionIndexFromCache)) << "should find entry";
    EXPECT_TRUE(intersectionIndexFromCache == 13) << "correct cache value should be retrieved";

}
