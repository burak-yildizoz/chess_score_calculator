#include <chess_score_calculator/tile.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <stdexcept>
////////////////////////////////////////////////////////////////////////////////

namespace chess {

Tile::Tile(Coordinate coordinate) noexcept
    : coordinate(coordinate)
{
}

Tile::Tile(std::unique_ptr<Piece> piece) noexcept
    : piece(std::move(piece))
{
}

bool Tile::has_piece() const noexcept
{
    return piece != nullptr;
}

Coordinate Tile::get_coordinate() const noexcept
{
    return has_piece() ? piece->get_coordinate() : coordinate;
}

Piece& Tile::get_piece() &
{
    if (!has_piece()) {
        throw std::logic_error("Tile has no piece");
    }
    return *piece;
}

const Piece& Tile::get_piece() const&
{
    return const_cast<Tile*>(this)->get_piece();
}

} // namespace chess
