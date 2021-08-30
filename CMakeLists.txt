cmake_minimum_required(VERSION 3.7.2)
project(graph_search)

add_library(graph_search INTERFACE)
target_include_directories(graph_search INTERFACE include)

add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox graph_search)
set_target_properties(sandbox PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED TRUE)