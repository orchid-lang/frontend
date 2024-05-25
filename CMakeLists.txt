# CMakeList.txt : CMake project for frontend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("frontend")

add_executable (frontend "main.cpp" )

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED true)

# TODO: Add tests and install targets if needed.
