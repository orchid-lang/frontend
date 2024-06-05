# CMakeList.txt : CMake project for frontend, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("frontend")

add_executable (frontend "main.cpp"  "lexer/lexer.cpp" "parser/parser.cpp")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED true)

enable_testing()

# Set compiler flag, diffrent on windows or linux
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(frontend PRIVATE /std:c++20)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_options(frontend PRIVATE -std=c++20)
else ()
    # sad freeBSD noises
    message(FATAL_ERROR "Unsupported platform for compiling: ${CMAKE_SYSTEM_NAME}")
endif ()

# Tests
add_executable(lexer_tests Tests/lexer_test.cpp lexer/lexer.cpp)
# add_executable(parser_tests Tests/parser_test.cpp parser/parser.cpp)

add_test(NAME LexerTest COMMAND lexer_tests)
# add_test(NAME ParserTest COMMAND parser_tests)
