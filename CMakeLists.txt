cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Tells Clangd how to read your project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(first_cmake_program
    lexer.h
    main.cpp
    lexer.cpp
    ast_maker.h
    ast_maker.cpp
    evaluate.h
    evaluate.cpp
)
