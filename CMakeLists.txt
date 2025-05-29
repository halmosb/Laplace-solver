cmake_minimum_required(VERSION 3.14)
project(laplace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native")

# === Dependencies ===
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(HDF5 REQUIRED CXX)

enable_testing()

# === Subdirectories ===
add_subdirectory(src)
add_subdirectory(test)
