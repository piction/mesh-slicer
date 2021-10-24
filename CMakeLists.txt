cmake_minimum_required(VERSION 3.13)

#option(testingEnabled "TESTING_ENABLED" OFF)
message(ENABLE_TESTS: ${ENABLE_TESTS})

set (THISLIB slicerLib)
set (THISPROGRAM slicer)

## setup project
project(THISLIB LANGUAGES CXX)
project(THISPROGRAM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)


set( SPDLOG_LIB_PATH_INC ${PROJECT_SOURCE_DIR}/lib/spdlog/include)

set(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_PATH ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_FILES 
                
                
                ${SRC_PATH}/log.cpp
                

                )


add_library(${THISLIB} STATIC ${SOURCE_FILES} )
#target_precompile_headers(${THISLIB} PUBLIC ${INCLUDE_PATH}/pch.h)
target_include_directories(${THISLIB} PRIVATE ${INCLUDE_PATH})


target_include_directories(${THISLIB} PRIVATE ${SPDLOG_LIB_PATH_INC})
target_link_libraries(${THISLIB} pthread)
target_sources(${THISLIB} PRIVATE ${SOURCE_FILES})

add_executable(${THISPROGRAM} main.cpp )
#target_precompile_headers(${THISPROGRAM} PUBLIC ${INCLUDE_PATH}/pch.h)
target_link_libraries(${THISPROGRAM} ${THISLIB}  )



target_include_directories(${THISPROGRAM} PRIVATE ${SPDLOG_LIB_PATH_INC})
target_include_directories(${THISPROGRAM} PRIVATE ${INCLUDE_PATH})

if(${ENABLE_TESTS}) 
    message(done testing)
    enable_testing()
    add_subdirectory(tests)
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/logs")

