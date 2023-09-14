#ifndef CHESS_SCORE_CALCULATOR_TILE_HPP
#define CHESS_SCORE_CALCULATOR_TILE_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <memory>
////////////////////////////////////////////////////////////////////////////////
// User Defined Libraries
#include <chess_score_calculator/enums.hpp>
#include <chess_score_calculator/piece.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace chess {

class Tile {
public:
    /// Default constructor, not intended for general use
    Tile() = default;

    /// Construct with no piece
    explicit Tile(Coordinate coordinate) noexcept;

    /// Construct with piece
    explicit Tile(std::unique_ptr<Piece> piece) noexcept;

    bool has_piece() const noexcept;

    Coordinate get_coordinate() const noexcept;

    /// @warning Throws if #has_piece is false
    Piece& get_piece() &;

    /// @overload
    const Piece& get_piece() const&;

    /// Avoid dangling reference
    /// @overload
    Piece get_piece() && = delete;

private:
    Coordinate coordinate;
    std::unique_ptr<Piece> piece;
};

} // namespace chess

#endif // CHESS_SCORE_CALCULATOR_TILE_HPP
