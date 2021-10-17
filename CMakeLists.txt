cmake_minimum_required(VERSION 3.20)
project(CHM2)

set(CMAKE_CXX_STANDARD 23)

add_executable(CHM2 main.cpp)

find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)
target_link_libraries(CHM2 sfml-graphics sfml-window sfml-system)