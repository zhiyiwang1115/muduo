cmake_minimum_required(VERSION 2.5)
project(server)

# dynamic pakcage would be put under ./lib
set(LIBRARY_OUTPUT_DIRECTORY ./lib)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14")

# define file to be compiled
aux_source_directory(./src SRC_LIST)

# generate dynamic package
add_library(server SHARED ${SRC_LIST})