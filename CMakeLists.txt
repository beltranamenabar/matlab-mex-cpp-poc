cmake_minimum_required(VERSION 3.16)

project(matlab-cpp-api)

find_package(Matlab)
matlab_add_mex(NAME convertToPolyshape SRC src/main.cpp)
