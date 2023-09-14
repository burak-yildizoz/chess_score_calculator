cmake_minimum_required(VERSION 3.0)
project(chess_score_calculator)

# set variables

set(cmake_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${cmake_DIR})

# set compiler options

include(compiler_options)

# chess_score_calculator_library

add_library(chess_score_calculator_library STATIC
    "src/chessboard.cpp" "include/chess_score_calculator/chessboard.hpp"
    "include/chess_score_calculator/enums.hpp"
    "src/piece.cpp" "include/chess_score_calculator/piece.hpp"
    "src/tile.cpp" "include/chess_score_calculator/tile.hpp"
)
target_include_directories(chess_score_calculator_library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
set_property(TARGET chess_score_calculator_library PROPERTY FOLDER "lib")

# chess_score_calculator

add_executable(chess_score_calculator "src/chess_score_calculator_main.cpp")
target_link_libraries(chess_score_calculator chess_score_calculator_library)
set_property(TARGET chess_score_calculator PROPERTY FOLDER "main")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT chess_score_calculator)
