cmake_minimum_required(VERSION 3.15)
project(flexilog)

set(CMAKE_CXX_STANDARD 20)

add_executable(flexilog flexilog_main.cpp)

install(TARGETS flexilog DESTINATION bin)

