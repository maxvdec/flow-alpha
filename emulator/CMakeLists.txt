
cmake_minimum_required(VERSION 3.15)

project(fca_compiler)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE FLOWCMP_SOURCES ${CMAKE_SOURCE_DIR}/flowalpha/*.c)

add_executable(flowalpha ${FLOWCMP_SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

