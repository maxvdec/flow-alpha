cmake_minimum_required(VERSION 3.15)

project(fca_compiler)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE FLOWCMP_SOURCES ${CMAKE_SOURCE_DIR}/flowcmp/*.cpp)
file(GLOB_RECURSE FLOWREVERSE_SOURCES ${CMAKE_SOURCE_DIR}/flowreverse/*.cpp)
file(GLOB_RECURSE FLOWINSPECT_SOURCES ${CMAKE_SOURCE_DIR}/flowinspect/*.cpp)

add_executable(flowcmp ${FLOWCMP_SOURCES})

add_executable(flowreverse ${FLOWREVERSE_SOURCES})

add_executable(flowinspect ${FLOWINSPECT_SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

