# CMakeList.txt : CMake project for RSA, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project ("RSA")

find_package(Boost COMPONENTS )  

# Add source to this project's executable.
add_executable (RSA "RSA.cpp" "RSA.h" "main.cpp")

# TODO: Add tests and install targets if needed.
