cmake_minimum_required(VERSION 3.23)
project(tinyraycaster)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES src/*.cpp src/*.hpp src/*.h)

file(COPY maps DESTINATION ${CMAKE_BINARY_DIR})

find_package(raylib 4.2 REQUIRED)

add_executable(tinyraycaster ${SOURCES})

target_link_libraries(tinyraycaster raylib)
