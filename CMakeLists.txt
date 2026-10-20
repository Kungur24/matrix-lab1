cmake_minimum_required(VERSION 3.20)
project(matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix
    main.cpp
    AppManager.cpp
    AppManager.h
    ConsoleHelper.cpp
    ConsoleHelper.h
    Line.cpp
    Line.h
    Symbol.cpp
    Symbol.h
)
