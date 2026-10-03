cmake_minimum_required(VERSION 3.25)

project(FableTLCModern LANGUAGES CXX)

include(CTest)

add_subdirectory(core)
add_subdirectory(assets)
add_subdirectory(multiplayer)
add_subdirectory(oracle)
