cmake_minimum_required(VERSION 3.30)

project(clox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE sources LIST_DIRECTORIES false src/*.cpp src/*.hpp)

add_executable(${PROJECT_NAME} ${sources})
if(CMAKE_CXX_COMPILER_ID STREQUAL Clang OR CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wconversion -Wunused)
endif()
