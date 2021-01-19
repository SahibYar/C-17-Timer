cmake_minimum_required(VERSION 3.12)
project(C17-Timer)
set(CMAKE_CXX_STANDARD 17)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} pthread)