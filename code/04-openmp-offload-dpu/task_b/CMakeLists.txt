cmake_minimum_required(VERSION 3.0.0)


set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "-fopenmp -fopenmp-targets=aarch64-unknown-linux")

project(omp_parallel)

add_executable(omp_parallel omp_parallel.c)

