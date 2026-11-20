cmake_minimum_required(VERSION 3.16)
project(matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(matrix
    main.cpp
    AppManager.cpp
    ConsoleHelper.cpp
    Line.cpp
    Symbol.cpp
    Explosion.cpp
)
