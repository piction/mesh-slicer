#include <gtest/gtest.h>
#include "log.h"



TEST(empty,basics ){
    Log::Init();
    LOG_INFO("empty test ");
    bool isTrue = true;
    EXPECT_TRUE(isTrue) << "dummy test!!";
}