#ifndef CHESS_SCORE_CALCULATOR_ENUMS_HPP
#define CHESS_SCORE_CALCULATOR_ENUMS_HPP

////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <compare>
#include <optional>
////////////////////////////////////////////////////////////////////////////////

namespace chess {

enum class Side {
    White,
    Black,
};

/// y coordinate at increasing order
enum class Row {
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
};

/// x coordinate at increasing order
enum class Column {
    a,
    b,
    c,
    d,
    e,
    f,
    g,
    h,
};

struct Coordinate {
    Row row;
    Column col;

    friend constexpr std::strong_ordering operator<=>(const Coordinate& lhs, const Coordinate& rhs) = default;
};

constexpr bool is_valid_coordinate(const Coordinate& coordinate) noexcept;

/**
Get the coordinate from given point

@param anchor Starting point
@param row_up #Row number to increase. A negative value means going below.
@param col_right #Column number to increase. A negative value means going left.
@returns Target coordinate, if inside chessboard.
*/
std::optional<Coordinate> get_coordinate_at(const Coordinate& anchor, int row_up, int col_right);

} // namespace chess

////////////////////////////////////////////////////////////////////////////////
// INLINE DEFINITIONS
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <optional>
////////////////////////////////////////////////////////////////////////////////

namespace chess {

constexpr bool is_valid_coordinate(const Coordinate& coordinate) noexcept
{
    const int row = static_cast<int>(coordinate.row);
    const int col = static_cast<int>(coordinate.col);
    constexpr int row_lower = static_cast<int>(Row::_1);
    constexpr int row_upper = static_cast<int>(Row::_8);
    constexpr int col_lower = static_cast<int>(Column::a);
    constexpr int col_upper = static_cast<int>(Column::h);
    const bool row_valid = (row_lower <= row) && (row <= row_upper);
    const bool col_valid = (col_lower <= col) && (col <= col_upper);
    return row_valid && col_valid;
}

inline std::optional<Coordinate> get_coordinate_at(const Coordinate& anchor, int row_up, int col_right)
{
    const int target_row = static_cast<int>(anchor.row) + row_up;
    const int target_col = static_cast<int>(anchor.col) + col_right;
    Coordinate result { static_cast<Row>(target_row), static_cast<Column>(target_col) };
    if (is_valid_coordinate(result)) {
        return result;
    } else {
        return std::nullopt;
    }
}

} // namespace chess

#endif // CHESS_SCORE_CALCULATOR_ENUMS_HPP
