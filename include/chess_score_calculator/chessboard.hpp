#ifndef CHESS_SCORE_CALCULATOR_CHESSBOARD_HPP
#define CHESS_SCORE_CALCULATOR_CHESSBOARD_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <array>
#include <filesystem>
#include <set>
////////////////////////////////////////////////////////////////////////////////
// User Defined Libraries
#include <chess_score_calculator/enums.hpp>
#include <chess_score_calculator/tile.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace chess {

class Chessboard {
public:
    /// @warning Throws if file is invalid
    explicit Chessboard(const std::filesystem::path& board_file);

    /// @warning Throws if coordinate is out of bounds
    Tile& get_tile_at(const Coordinate& coordinate) &;

    /// @overload
    const Tile& get_tile_at(const Coordinate& coordinate) const&;

    /// Avoid dangling reference
    /// @overload
    Tile get_tile_at(const Coordinate& coordinate) && = delete;

    std::set<Coordinate> get_white_piece_coordinates() const;
    std::set<Coordinate> get_black_piece_coordinates() const;
    std::set<Coordinate> get_all_piece_coordinates() const;

    std::set<Coordinate> get_threatened_white_piece_coordinates() const;
    std::set<Coordinate> get_threatened_black_piece_coordinates() const;

    std::set<Coordinate> get_unthreatened_white_piece_coordinates() const;
    std::set<Coordinate> get_unthreatened_black_piece_coordinates() const;

    double score_of_whites() const;
    double score_of_blacks() const;

private:
    std::array<std::array<Tile, 8>, 8> tiles;
};

} // namespace chess

#endif // CHESS_SCORE_CALCULATOR_CHESSBOARD_HPP
