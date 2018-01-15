cmake_minimum_required (VERSION 2.8)

project (Demo1)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -pthread")
add_executable(Demo main.cc)
