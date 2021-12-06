# CMakeList.txt : CMake project for RSA, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.20)

project ("RSA")

find_package(Boost COMPONENTS)

include_directories(${PROJECT_SOURCE_DIR}/Turbo-Base64)

# Add source to this project's executable.
add_executable (RSA "RSA.cpp" "RSA.h" "main.cpp" "base64.h" "base64.cpp")

# TODO: Add tests and install targets if needed.
