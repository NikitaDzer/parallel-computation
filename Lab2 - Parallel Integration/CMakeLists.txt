cmake_minimum_required(VERSION 3.10)
project(ParLab2)

set(CMAKE_CXX_FLAGS "-Ofast")

# PINT application.
add_executable(pint main.cc)
target_include_directories(pint PUBLIC include)
target_compile_features(pint PUBLIC cxx_std_20)
