# CMakeList.txt : CMake project for frontend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("frontend")

add_executable (frontend "main.cpp"  "lexer/lexer.cpp")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED true)

target_compile_options(frontend PRIVATE /std:c++20)

# TODO: Add tests and install targets if needed.
