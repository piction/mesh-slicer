# Goal of the program

The program will slice a waveFormat obj file containing a mesh with a given plane.


# Building 
This project uses CMAKE to setup the build environment. 
There is a secondary CMakeList file for the testing. 

````
cmake DCMAKE_CXX_FLAGS=-Wall  -DENABLE_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..
````


# Running 
The program accepts 2 arguments referencing two files. One is the meshdata (obj file) and the other is a description of the cutting plane (json-file).

````
./slicer "tests/testData/bunny.obj tests/testData/plane-description.json"
````
