cmake_minimum_required(VERSION 3.24)
project(Program_1 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Program_1
        BMPHandler.h
        Image.h BMPHandler.c Image.c LastNameImageProcessor.c)
