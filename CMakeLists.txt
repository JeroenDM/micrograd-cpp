cmake_minimum_required(VERSION 3.21)
project(
  MicrogradCpp
  VERSION 1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -pedantic -Wconversion)

add_executable(example example.cpp)

target_include_directories(example PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Clean target
add_custom_target(
  clean-all
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/compile_commands.json
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/Makefile
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/cmake_install.cmake)

# Create cmake_clean.cmake script for cleaning
file(WRITE ${CMAKE_BINARY_DIR}/cmake_clean.cmake
     "file(REMOVE_RECURSE \${CMAKE_BINARY_DIR}/example)\n")
