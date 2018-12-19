cmake_minimum_required(VERSION 3.13)
project(My_Language)

set(CMAKE_CXX_STANDARD 14)

add_executable(My_Language main.cpp Code_tree.h Code_tree.cpp ASM_Generator.h ASM_Generator.cpp)