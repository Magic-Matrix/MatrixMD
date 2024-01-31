# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

# Set the project name
project (matrix_md)

set(MD_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

include_directories(${MD_INCLUDE_DIR})


file(GLOB_RECURSE MD_LIB_SRC src/*.cpp)
# message(${MD_LIB_SRC})

# Add an executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${MD_LIB_SRC}
)

# target_include_directories(
#     ${PROJECT_NAME}
#     PRIVATE 
#     MD_INCLUDE_DIR
# )