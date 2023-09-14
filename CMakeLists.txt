cmake_minimum_required(VERSION 3.0)
project(chess_score_calculator)

set(cmake_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${cmake_DIR})

include(compiler_options)

add_executable(chess_score_calculator "src/chess_score_calculator_main.cpp")
