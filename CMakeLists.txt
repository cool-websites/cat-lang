cmake_minimum_required(VERSION 3.10)
project(cat C)

set(CMAKE_C_STANDARD 11)

add_executable(cat
    src/lexer.c
    src/interpreter.c
    src/main.c
)

target_include_directories(cat PRIVATE include)