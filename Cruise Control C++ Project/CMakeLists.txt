cmake_minimum_required(VERSION 3.10)
project(Group_38_Cruise_Control)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories (optional)
include_directories(include)

# This will find all .cpp files recursively under the 'src' directory
file(GLOB SOURCES "src/*.cpp")

# Add the executable
add_executable(Cruise_Control_Sprint_1 ${SOURCES})
