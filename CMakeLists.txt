cmake_minimum_required(VERSION 3.10)
project(ConwaysGameOfLife)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(game_of_life main.cpp)

# Add compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(game_of_life PRIVATE -Wall -Wextra -pedantic)
elseif(MSVC)
    target_compile_options(game_of_life PRIVATE /W4)
endif()

# Find threads package for sleep functionality
find_package(Threads REQUIRED)
target_link_libraries(game_of_life PRIVATE Threads::Threads)