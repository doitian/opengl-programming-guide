cmake_minimum_required(VERSION 3.29)
project(opg C)

find_package(glfw3 REQUIRED)

add_executable(a1 src/a1.c)
target_link_libraries(a1 glfw)
