#include <chess_score_calculator/chessboard.hpp>
////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <execution>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <numeric>
#include <ranges>
#include <stdexcept>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Libraries
#include <chess_score_calculator/enums.hpp>
#include <chess_score_calculator/piece.hpp>
////////////////////////////////////////////////////////////////////////////////

namespace {

/// Function object to throw invalid_argument at Chessboard constructor
class ThrowInvalidArgument {
public:
    explicit ThrowInvalidArgument(const std::filesystem::path& board_file) noexcept
        : board_file(board_file)
    {
    }

    [[noreturn]] void operator()(std::string description, std::string denotation) const
    {
        throw std::invalid_argument(description + " [" + denotation + "] at file " + board_file.filename().string());
    }

    const std::filesystem::path& board_file;
};

/// Function object to convert denotation to Side or throw invalid argument
class GetSide {
public:
    using Side = chess::Side;

    explicit GetSide(const ThrowInvalidArgument& throw_invalid_argument) noexcept
        : throw_invalid_argument(throw_invalid_argument)
    {
    }

    Side operator()(char ch) const
    {
        switch (ch) {
        case 'b':
            return Side::White;
        case 's':
            return Side::Black;
        default:
            throw_invalid_argument("Invalid side denotation character", std::string { ch });
        }
    }

    const ThrowInvalidArgument& throw_invalid_argument;
};

/// Function object to convert denotation to Piece or throw invalid argument
class GetPiece {
public:
    using Chessboard = chess::Chessboard;
    using Piece = chess::Piece;
    using Coordinate = chess::Coordinate;
    using Side = chess::Side;

    explicit GetPiece(const Chessboard& chessboard, const ThrowInvalidArgument& throw_invalid_argument) noexcept
        : chessboard(chessboard)
        , throw_invalid_argument(throw_invalid_argument)
    {
    }

    std::unique_ptr<Piece> operator()(char ch, const Coordinate& coordinate, Side side) const
    {
        using namespace chess;
        switch (ch) {
        case 'p':
            return std::make_unique<Pawn>(coordinate, side, chessboard);
        case 'a':
            return std::make_unique<Knight>(coordinate, side, chessboard);
        case 'f':
            return std::make_unique<Bishop>(coordinate, side, chessboard);
        case 'k':
            return std::make_unique<Rook>(coordinate, side, chessboard);
        case 'v':
            return std::make_unique<Queen>(coordinate, side, chessboard);
        case 's':
            return std::make_unique<King>(coordinate, side, chessboard);
        default:
            throw_invalid_argument("Invalid piece denotation character", std::string { ch });
        }
    }

    const Chessboard& chessboard;
    const ThrowInvalidArgument& throw_invalid_argument;
};

/// Function object to get the score of a Piece at given Coordinate
class GetScore {
public:
    using Chessboard = chess::Chessboard;
    using Coordinate = chess::Coordinate;

    explicit GetScore(const Chessboard& chessboard) noexcept
        : chessboard(chessboard)
    {
    }

    double operator()(const Coordinate& coordinate) const
    {
        return chessboard.get_tile_at(coordinate).get_piece().get_unthreatened_score();
    }

    const Chessboard& chessboard;
};

} // namespace

////////////////////////////////////////////////////////////////////////////////

namespace chess {

Chessboard::Chessboard(const std::filesystem::path& board_file)
{
    // check if file exists
    if (!std::filesystem::is_regular_file(board_file)) {
        throw std::runtime_error("File not found: " + board_file.string());
    }
    // open file by exceptions enabled
    std::ifstream ifs;
    ifs.exceptions(std::ios_base::badbit | std::ios_base::failbit);
    ifs.open(board_file);
    // create function object instances
    const ThrowInvalidArgument throw_invalid_argument(board_file);
    const GetSide get_side(throw_invalid_argument);
    const GetPiece get_piece(*this, throw_invalid_argument);
    // perform int - Coordinate conversions
    constexpr int row_start = static_cast<int>(Row::_1);
    constexpr int row_end = static_cast<int>(Row::_8);
    constexpr int col_start = static_cast<int>(Column::a);
    constexpr int col_end = static_cast<int>(Column::h);
    // fill all tiles
    for (int row = row_end; row >= row_start; row--) {
        for (int col = col_start; col <= col_end; col++) {
            // get denotation of tile
            std::string tile_denotation;
            ifs >> tile_denotation;
            // denotation must consist of 2 characters
            if (tile_denotation.length() != 2) {
                throw_invalid_argument("Invalid tile denotation", tile_denotation);
            }
            // current coordinate point
            const Coordinate coordinate { static_cast<Row>(row), static_cast<Column>(col) };
            // current index
            const int row_index = row - row_start;
            const int col_index = col - col_start;
            // check empty tile
            if (tile_denotation == "--") {
                tiles.at(row_index).at(col_index) = Tile(coordinate);
                continue;
            }
            // get side from second denotation character
            const Side side = get_side(tile_denotation[1]);
            // get piece from first denotation character
            std::unique_ptr<Piece> piece = get_piece(tile_denotation[0], coordinate, side);
            // assign to tile
            tiles.at(row_index).at(col_index) = Tile(std::move(piece));
        }
    }
}

Tile& Chessboard::get_tile_at(const Coordinate& coordinate) &
{
    if (!is_valid_coordinate(coordinate)) {
        throw std::invalid_argument("Provided coordinate is not valid");
    }
    constexpr int row_start = static_cast<int>(Row::_1);
    constexpr int col_start = static_cast<int>(Column::a);
    const int row_index = static_cast<int>(coordinate.row) - row_start;
    const int col_index = static_cast<int>(coordinate.col) - col_start;
    return tiles.at(row_index).at(col_index);
}

const Tile& Chessboard::get_tile_at(const Coordinate& coordinate) const&
{
    return const_cast<Chessboard*>(this)->get_tile_at(coordinate);
}

std::set<Coordinate> Chessboard::get_white_piece_coordinates() const
{
    std::set<Coordinate> result;
    for (const std::array<Tile, 8>& row : tiles) {
        for (const Tile& tile : row) {
            if (tile.has_piece() && (tile.get_piece().get_side() == Side::White)) {
                result.emplace(tile.get_coordinate());
            }
        }
    }
    return result;
}

std::set<Coordinate> Chessboard::get_black_piece_coordinates() const
{
    std::set<Coordinate> result;
    for (const std::array<Tile, 8>& row : tiles) {
        for (const Tile& tile : row) {
            if (tile.has_piece() && (tile.get_piece().get_side() == Side::Black)) {
                result.emplace(tile.get_coordinate());
            }
        }
    }
    return result;
}

std::set<Coordinate> Chessboard::get_all_piece_coordinates() const
{
    std::set<Coordinate> result = get_white_piece_coordinates();
    result.merge(get_black_piece_coordinates());
    return result;
}

std::set<Coordinate> Chessboard::get_threatened_white_piece_coordinates() const
{
    std::set<Coordinate> result;
    for (Coordinate black_coordinate : get_black_piece_coordinates()) {
        const Piece& black_piece = get_tile_at(black_coordinate).get_piece();
        result.merge(black_piece.get_threated_piece_coordinates());
    }
    return result;
}

std::set<Coordinate> Chessboard::get_threatened_black_piece_coordinates() const
{
    std::set<Coordinate> result;
    for (Coordinate white_coordinate : get_white_piece_coordinates()) {
        const Piece& white_piece = get_tile_at(white_coordinate).get_piece();
        result.merge(white_piece.get_threated_piece_coordinates());
    }
    return result;
}

std::set<Coordinate> Chessboard::get_unthreatened_white_piece_coordinates() const
{
    const std::set<Coordinate> all = get_white_piece_coordinates();
    const std::set<Coordinate> threatened = get_threatened_white_piece_coordinates();
    std::set<Coordinate> result;
    std::ranges::set_difference(all, threatened, std::inserter(result, result.begin()));
    return result;
}

std::set<Coordinate> Chessboard::get_unthreatened_black_piece_coordinates() const
{
    const std::set<Coordinate> all = get_black_piece_coordinates();
    const std::set<Coordinate> threatened = get_threatened_black_piece_coordinates();
    std::set<Coordinate> result;
    std::ranges::set_difference(all, threatened, std::inserter(result, result.begin()));
    return result;
}

double Chessboard::score_of_whites() const
{
    // create function object instance
    const GetScore get_score(*this);
    // get white coordinates
    const std::set<Coordinate> unthreatened_coordinates = get_unthreatened_white_piece_coordinates();
    const std::set<Coordinate> threatened_coordinates = get_threatened_white_piece_coordinates();
    // calculate unthreatened score
    std::vector<double> unthreatened_scores;
    unthreatened_scores.reserve(unthreatened_coordinates.size());
    std::ranges::transform(unthreatened_coordinates, std::back_inserter(unthreatened_scores), get_score);
    const double unthreatened_score = std::reduce(std::execution::par_unseq, unthreatened_scores.begin(), unthreatened_scores.end());
    // calculate threatened score
    std::vector<double> threatened_scores;
    threatened_scores.reserve(threatened_coordinates.size());
    std::ranges::transform(threatened_coordinates, std::back_inserter(threatened_scores), get_score);
    const double threatened_score = std::reduce(std::execution::par_unseq, threatened_scores.begin(), threatened_scores.end()) / 2.0;
    return unthreatened_score + threatened_score;
}

double Chessboard::score_of_blacks() const
{
    // create function object instance
    const GetScore get_score(*this);
    // get black coordinates
    const std::set<Coordinate> unthreatened_coordinates = get_unthreatened_black_piece_coordinates();
    const std::set<Coordinate> threatened_coordinates = get_threatened_black_piece_coordinates();
    // calculate unthreatened score
    std::vector<double> unthreatened_scores;
    unthreatened_scores.reserve(unthreatened_coordinates.size());
    std::ranges::transform(unthreatened_coordinates, std::back_inserter(unthreatened_scores), get_score);
    const double unthreatened_score = std::reduce(std::execution::par_unseq, unthreatened_scores.begin(), unthreatened_scores.end());
    // calculate threatened score
    std::vector<double> threatened_scores;
    threatened_scores.reserve(threatened_coordinates.size());
    std::ranges::transform(threatened_coordinates, std::back_inserter(threatened_scores), get_score);
    const double threatened_score = std::reduce(std::execution::par_unseq, threatened_scores.begin(), threatened_scores.end()) / 2.0;
    return unthreatened_score + threatened_score;
}

} // namespace chess
