cmake_minimum_required(VERSION 3.5.1)

project(ConcurrencyErrorBenchmarkSuite)
set(ConcurrencyErrorBenchmarkSuite_VERSION_MAJOR 0)
set(ConcurrencyErrorBenchmarkSuite_VERSION_MINOR 1)

#set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
add_subdirectory(OpenMP)
#add_subdirectory(CUDA)
