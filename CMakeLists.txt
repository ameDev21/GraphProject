# suppress warnings
cmake_minimum_required(VERSION 3.21)

project(megaman)

# use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set compilation flags 
set(CMAKE_CXX_FLAGS "-O3 -Wall")

# specify the non-header source files that should be compiled
file(
  GLOB CPP_SOURCE
  "${PROJECT_SOURCE_DIR}/src/*.cpp" # i.e. `src/main.cpp`, but not `src/d/f.cpp`
  "${PROJECT_SOURCE_DIR}/src/code/*.cpp" # i.e. `src/code/f.cpp`, but not `src/d/f/k.cpp`
    "${PROJECT_SOURCE_DIR}/src/code/Algorithms/*.cpp" # i.e. `src/code/f.cpp`, but not `src/d/f/k.cpp`
)

# where to find header files
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# name of the executable to be created, and which source files to use
add_executable(main ${CPP_SOURCE})

# find sfml package
find_package(SFML 2.5 COMPONENTS graphics)

# make sfml works
target_link_libraries(main sfml-graphics)
