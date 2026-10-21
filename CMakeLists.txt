cmake_minimum_required(VERSION 3.25)
include(FetchContent)

project(DynamicInputBuffer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geode REQUIRED)

add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    src/buffer_engine.cpp
    src/hooks/game_layer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

setup_geode_mod(${PROJECT_NAME})