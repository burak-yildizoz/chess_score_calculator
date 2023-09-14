cmake_minimum_required(VERSION 3.0)
project(chess_score_calculator)

set(cmake_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${cmake_DIR})

include(compiler_options)

add_library(chess_score_calculator_library STATIC
    "include/chess_score_calculator/enums.hpp"
    "include/chess_score_calculator/piece.hpp"
    "src/tile.cpp" "include/chess_score_calculator/tile.hpp"
)
target_include_directories(chess_score_calculator_library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(chess_score_calculator "src/chess_score_calculator_main.cpp")
target_link_libraries(chess_score_calculator chess_score_calculator_library)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT chess_score_calculator)
