project(btlink)
cmake_minimum_required(VERSION 3.8)

add_compile_options (-fdiagnostics-color=always)
include_directories(include)

add_subdirectory(apps)
add_subdirectory(core)

enable_testing()
add_subdirectory(test)
