cmake_minimum_required(VERSION 3.10)
project(better-lists C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(-Wall)

add_library(list STATIC src/list.c)
target_include_directories(list PUBLIC src)

add_executable(tests src/tests.c)
target_link_libraries(tests PRIVATE list)
