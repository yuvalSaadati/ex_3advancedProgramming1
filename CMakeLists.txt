cmake_minimum_required(VERSION 3.15)
project(ex33)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -pthread)
add_executable(ex33 main.cpp CommandInterface.h commandPattern.cpp commandPattern.h Singleton.cpp Singleton.h)