#include <chess_score_calculator/piece.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <array>
#include <optional>
#include <utility>
////////////////////////////////////////////////////////////////////////////////
// User Defined Libraries
#include <chess_score_calculator/chessboard.hpp>
#include <chess_score_calculator/enums.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace chess {

Piece::Piece(Coordinate coordinate, Side side, const Chessboard& parent) noexcept
    : coordinate(coordinate)
    , side(side)
    , chessboard(parent)
{
}

Coordinate Piece::get_coordinate() const noexcept
{
    return coordinate;
}

Side Piece::get_side() const noexcept
{
    return side;
}

bool Piece::coordinate_has_threated_piece(const Coordinate& target_coordinate) const
{
    const Tile& tile = chessboard.get_tile_at(target_coordinate);
    if (!tile.has_piece()) {
        return false;
    }
    return tile.get_piece().get_side() != get_side();
}

std::optional<Coordinate> Piece::find_piece_at_direction(int row_up, int col_right) const
{
    std::optional<Coordinate> result = get_coordinate();
    do {
        result = get_coordinate_at(*result, row_up, col_right);
        if (result && chessboard.get_tile_at(*result).has_piece()) {
            break;
        }
    } while (result);
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Pawn::Pawn(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept
    : Piece(std::move(coordinate), side, chessboard)
{
}

std::set<Coordinate> Pawn::get_threated_piece_coordinates() const
{
    const int row_up = (get_side() == Side::White) ? 1 : -1;
    const std::array coordinates_to_check = {
        get_coordinate_at(get_coordinate(), row_up, 1),
        get_coordinate_at(get_coordinate(), row_up, -1),
    };
    std::set<Coordinate> result;
    for (const std::optional<Coordinate>& target_coordinate : coordinates_to_check) {
        if (target_coordinate && coordinate_has_threated_piece(*target_coordinate)) {
            result.emplace(*target_coordinate);
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Knight::Knight(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept
    : Piece(std::move(coordinate), side, chessboard)
{
}

std::set<Coordinate> Knight::get_threated_piece_coordinates() const
{
    constexpr std::array distances_to_check = {
        std::pair { 1, 2 },
        std::pair { 1, -2 },
        std::pair { -1, 2 },
        std::pair { -1, -2 },
        std::pair { 2, 1 },
        std::pair { 2, -1 },
        std::pair { -2, 1 },
        std::pair { -2, -1 },
    };
    std::set<Coordinate> result;
    for (const std::pair<int, int>& distance : distances_to_check) {
        const std::optional<Coordinate> target_coordinate = get_coordinate_at(get_coordinate(), distance.first, distance.second);
        if (target_coordinate && coordinate_has_threated_piece(*target_coordinate)) {
            result.emplace(*target_coordinate);
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Bishop::Bishop(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept
    : Piece(std::move(coordinate), side, chessboard)
{
}

std::set<Coordinate> Bishop::get_threated_piece_coordinates() const
{
    const std::array coordinates_to_check = {
        find_piece_at_direction(1, 1),
        find_piece_at_direction(1, -1),
        find_piece_at_direction(-1, 1),
        find_piece_at_direction(-1, -1),
    };
    std::set<Coordinate> result;
    for (const std::optional<Coordinate>& target_coordinate : coordinates_to_check) {
        if (target_coordinate && coordinate_has_threated_piece(*target_coordinate)) {
            result.emplace(*target_coordinate);
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Rook::Rook(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept
    : Piece(std::move(coordinate), side, chessboard)
{
}

std::set<Coordinate> Rook::get_threated_piece_coordinates() const
{
    const std::array coordinates_to_check = {
        find_piece_at_direction(1, 0),
        find_piece_at_direction(-1, 0),
        find_piece_at_direction(0, 1),
        find_piece_at_direction(0, -1),
    };
    std::set<Coordinate> result;
    for (const std::optional<Coordinate>& target_coordinate : coordinates_to_check) {
        if (target_coordinate && coordinate_has_threated_piece(*target_coordinate)) {
            result.emplace(*target_coordinate);
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////

Queen::Queen(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept
    : Piece(std::move(coordinate), side, chessboard)
{
}

std::set<Coordinate> Queen::get_threated_piece_coordinates() const
{
    const std::array coordinates_to_check = {
        find_piece_at_direction(1, 0),
        find_piece_at_direction(-1, 0),
        find_piece_at_direction(0, 1),
        find_piece_at_direction(0, -1),
        find_piece_at_direction(1, 1),
        find_piece_at_direction(1, -1),
        find_piece_at_direction(-1, 1),
        find_piece_at_direction(-1, -1),
    };
    std::set<Coordinate> result;
    for (const std::optional<Coordinate>& target_coordinate : coordinates_to_check) {
        if (target_coordinate && coordinate_has_threated_piece(*target_coordinate)) {
            result.emplace(*target_coordinate);
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////

King::King(Coordinate coordinate, Side side, const Chessboard& chessboard) noexcept
    : Piece(std::move(coordinate), side, chessboard)
{
}

std::set<Coordinate> King::get_threated_piece_coordinates() const
{
    constexpr std::array distances_to_check = {
        std::pair { 1, 0 },
        std::pair { -1, 0 },
        std::pair { 0, 1 },
        std::pair { 0, -1 },
        std::pair { 1, 1 },
        std::pair { 1, -1 },
        std::pair { -1, 1 },
        std::pair { -1, -1 },
    };
    std::set<Coordinate> result;
    for (const std::pair<int, int>& distance : distances_to_check) {
        const std::optional<Coordinate> target_coordinate = get_coordinate_at(get_coordinate(), distance.first, distance.second);
        if (target_coordinate && coordinate_has_threated_piece(*target_coordinate)) {
            result.emplace(*target_coordinate);
        }
    }
    return result;
}

} // namespace chess
