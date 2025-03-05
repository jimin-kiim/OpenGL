# https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-2-specifying-the-c-standard
# https://www.cs.swarthmore.edu/~adanner/tips/cmake.php

file(GLOB SOURCES "*.cpp")

cmake_minimum_required(VERSION 3.10)
project(trial VERSION 1.0)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(trial ${SOURCES})

target_link_libraries(trial OpenGL::GL GLUT::GLUT)