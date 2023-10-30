cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "-fopenmp -fopenmp-targets=aarch64-unknown-linux")
set(INCLUDE_PATH  "${LLVM_PATH}/include/")

project(hello)

add_executable(hello hello.c)

target_include_directories(
	hello
	PRIVATE
	${INCLUDE_PATH}	
)

