cmake_minimum_required(VERSION 3.10)
project(ParLab1)

find_package(MPI REQUIRED)

# Static GNUPlot library.
set(VISUAL_SOURCES src/plot.cc)
add_library(visual ${VISUAL_SOURCES})
target_include_directories(visual PUBLIC include)
target_compile_features(visual PUBLIC cxx_std_20)

# PDE application.
add_executable(pde main.cc src/scheme.cc src/drawer.cc src/solver.cc)
target_include_directories(pde PUBLIC include)
target_compile_features(pde PUBLIC cxx_std_20)
target_link_libraries(pde PRIVATE visual MPI::MPI_CXX)

# PI calculator.
add_executable(pi pi.cc)
target_compile_features(pi PUBLIC cxx_std_20)
target_link_libraries(pi PRIVATE MPI::MPI_CXX)
