#ifndef CHESS_SCORE_CALCULATOR_PIECE_HPP
#define CHESS_SCORE_CALCULATOR_PIECE_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <optional>
#include <set>
////////////////////////////////////////////////////////////////////////////////
// User Defined Libraries
#include <chess_score_calculator/enums.hpp>
////////////////////////////////////////////////////////////////////////////////
// Forward Declarations
namespace chess {
class Chessboard;
}
////////////////////////////////////////////////////////////////////////////////

namespace chess {

/// Base class of all chess pieces
class Piece {
public:
    /// Each piece has a coordinate at the board and belongs to an opposing side
    Piece(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    virtual ~Piece() = default;

    Coordinate get_coordinate() const noexcept;
    Side get_side() const noexcept;

    virtual constexpr double get_unthreatened_score() const noexcept = 0;

    /// Get a list of threated pieces by this instance
    virtual std::set<Coordinate> get_threated_piece_coordinates() const = 0;

protected:
    /// @returns True if target coordinate has opponent piece
    bool coordinate_has_threated_piece(const Coordinate& target_coordinate) const;

    /**
    @param row_up Row number to increase. A negative value means going below.
    @param col_right Column number to increase. A negative value means going left.
    @see get_coordinate_at
    @returns Non-empty coordinate if any piece is found at given direction
    */
    std::optional<Coordinate> find_piece_at_direction(int row_up, int col_right) const;

    const Chessboard& chessboard;

private:
    Coordinate coordinate;
    Side side;
};

////////////////////////////////////////////////////////////////////////////////

class Pawn : public Piece {
public:
    Pawn(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    constexpr double get_unthreatened_score() const noexcept override { return 1; }

    std::set<Coordinate> get_threated_piece_coordinates() const override;
};

////////////////////////////////////////////////////////////////////////////////

class Knight : public Piece {
public:
    Knight(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    constexpr double get_unthreatened_score() const noexcept override { return 3; }

    std::set<Coordinate> get_threated_piece_coordinates() const override;
};

////////////////////////////////////////////////////////////////////////////////

class Bishop : public Piece {
public:
    Bishop(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    constexpr double get_unthreatened_score() const noexcept override { return 3; }

    std::set<Coordinate> get_threated_piece_coordinates() const override;
};

////////////////////////////////////////////////////////////////////////////////

class Rook : public Piece {
public:
    Rook(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    constexpr double get_unthreatened_score() const noexcept override { return 5; }

    std::set<Coordinate> get_threated_piece_coordinates() const override;
};

////////////////////////////////////////////////////////////////////////////////

class Queen : public Piece {
public:
    Queen(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    constexpr double get_unthreatened_score() const noexcept override { return 9; }

    std::set<Coordinate> get_threated_piece_coordinates() const override;
};

////////////////////////////////////////////////////////////////////////////////

class King : public Piece {
public:
    King(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept;

    constexpr double get_unthreatened_score() const noexcept override { return 100; }

    std::set<Coordinate> get_threated_piece_coordinates() const override;
};

} // namespace chess

#endif // CHESS_SCORE_CALCULATOR_PIECE_HPP
