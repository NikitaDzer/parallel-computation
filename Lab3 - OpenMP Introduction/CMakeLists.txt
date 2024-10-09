cmake_minimum_required(VERSION 3.10)
project(ParLab3)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_FLAGS "-Ofast")

# MAT application.
add_executable(mat main.cc)
target_include_directories(mat PUBLIC include)
target_compile_features(mat PUBLIC cxx_std_20)
target_link_libraries(mat PRIVATE OpenMP::OpenMP_CXX)

