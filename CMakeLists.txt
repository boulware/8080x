cmake_minimum_required(VERSION 3.5.1)
project(8080x)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

include_directories(include)
file(GLOB SOURCES "source/*.cpp")

add_executable(main ${SOURCES})