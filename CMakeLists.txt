cmake_minimum_required(VERSION 3.10)
project(ConwaysGameOfLife)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Add the executable
add_executable(game_of_life main.cpp)

# Link OpenMP
target_link_libraries(game_of_life PRIVATE OpenMP::OpenMP_CXX)

# Add compiler options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(game_of_life PRIVATE -Wall -Wextra -pedantic)
elseif(MSVC)
    target_compile_options(game_of_life PRIVATE /W4)
endif()

# Find threads package for sleep functionality
find_package(Threads REQUIRED)
target_link_libraries(game_of_life PRIVATE Threads::Threads)

# Optional: Set OpenMP flags manually if find_package doesn't work
# if(OPENMP_FOUND)
#     target_compile_options(game_of_life PRIVATE ${OpenMP_CXX_FLAGS})
#     target_link_libraries(game_of_life PRIVATE ${OpenMP_CXX_LIBRARIES})
# endif()