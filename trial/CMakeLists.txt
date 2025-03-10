# https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-2-specifying-the-c-standard
# https://www.cs.swarthmore.edu/~adanner/tips/cmake.php

cmake_minimum_required(VERSION 3.11) # FetchContent is new in version 3.11.

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include/)

file(GLOB SOURCES cube_glfw.cpp src/glad.c)

cmake_minimum_required(VERSION 3.10)
project(trial VERSION 1.0)

find_package(OpenGL REQUIRED)
# find_package(GLUT REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(trial ${SOURCES})

# target_link_libraries(trial OpenGL::GL GLUT::GLUT)
target_link_libraries(trial OpenGL::GL glfw glad glm::glm)