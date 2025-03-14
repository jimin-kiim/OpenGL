# https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-2-specifying-the-c-standard
# https://www.cs.swarthmore.edu/~adanner/tips/cmake.php

cmake_minimum_required(VERSION 3.11) # FetchContent is new in version 3.11.
project(skybox VERSION 1.0)
set(CMAKE_INCLUDE_PATH "/usr/include")

include(FetchContent)

# downloading glm
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# set(GLEW_INCLUDE_DIR "/usr/include") # GLEW_INCLUDE_DIR: is for where to find the glew.h
# find_package(GLEW REQUIRED HINTS "/usr/local") # providing additional location for finding the glew.h
# include_directories(${GLEW_INCLUDE_DIR})

# using OpenGL, glfw, glew, glm and assimp instead of glut.
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED) # Open Asset Import Library; for loading and processing 3D model files; OBJ 
# find_package(glm REQUIRED)
message("directory for finding GLEW file is set as : ${GLEW_INCLUDE_DIRS}") # -> /usr/include 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(skybox main.cpp)

# using OpenGL, glfw, glew instead of glut.
target_link_libraries(skybox OpenGL::GL glfw GLEW:GLEW assimp:assimp glm::glm)