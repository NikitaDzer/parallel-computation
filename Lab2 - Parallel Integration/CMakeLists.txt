cmake_minimum_required(VERSION 3.10)
project(ParLab2)

set(CMAKE_CXX_FLAGS "-Ofast -flto -fwhole-program -fsanitize=address")

# PINT application.
add_executable(pint main.cc seq.cc par.cc)
target_include_directories(pint PUBLIC include)
target_compile_features(pint PUBLIC cxx_std_20)
