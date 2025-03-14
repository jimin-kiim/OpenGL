# https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-2-specifying-the-c-standard
# https://www.cs.swarthmore.edu/~adanner/tips/cmake.php

cmake_minimum_required(VERSION 3.11) # FetchContent is new in version 3.11.
project(skybox VERSION 1.0)
set(CMAKE_INCLUDE_PATH "/usr/include")

# using OpenGL, glfw, glew, glm and assimp instead of glut.
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(assimp REQUIRED) # Open Asset Import Library; for loading and processing 3D model files; OBJ 
# find_package(glm REQUIRED)
message("directory for finding GLEW file is set as : ${GLEW_INCLUDE_DIRS}") # -> /usr/include 

# downloading glm
include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(SOURCES "*.cpp")
file(GLOB SOURCES "*.cpp")
add_executable(skybox ${SOURCES})

# using OpenGL, glfw, glew instead of glut.
target_link_libraries(skybox OpenGL::GL glfw GLEW::GLEW assimp::assimp glm::glm)