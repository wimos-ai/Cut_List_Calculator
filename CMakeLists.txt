cmake_minimum_required(VERSION 3.14)

# Project name and settings
project(CutListCalculator VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the source file(s)
set(SOURCES src/main.cpp src/cut_optimization_solver.cpp src/json_problem_parser.cpp)

# Add Nlohmann JSON as an external library
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2 # Use a specific version
)

FetchContent_MakeAvailable(nlohmann_json)

# Add the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include the custom include directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link Nlohmann JSON to your project
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
