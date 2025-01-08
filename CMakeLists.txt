# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project definition
project(MyProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src/util)  # Build util library
add_subdirectory(src/net)   # Build net library
add_subdirectory(src/thread)   # Build thread library
