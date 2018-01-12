cmake_minimum_required(VERSION 3.5.1)
project(8080x)
set (CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "source/*.cpp")

add_executable(main ${SOURCES})