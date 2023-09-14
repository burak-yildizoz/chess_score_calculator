////////////////////////////////////////////////////////////////////////////////
// Standard Libraries
#include <algorithm>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
// User Defined Libraries
#include <chess_score_calculator/chessboard.hpp>
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
try {
    // check argument provided
    if (argc == 1) {
        std::clog << "Usage: chess_score_calculator.exe board.txt ...\n";
        return 1;
    }
    // obtain input file paths
    const int num_boards = argc - 1;
    std::vector<std::filesystem::path> board_paths(num_boards);
    for (int i = 1; i < argc; i++) {
        board_paths.at(i - 1) = argv[i];
    }
    // calculate scores
    std::vector<double> white_scores;
    white_scores.reserve(num_boards);
    std::vector<double> black_scores;
    black_scores.reserve(num_boards);
    for (int i = 0; i < num_boards; i++) {
        chess::Chessboard chessboard(board_paths.at(i));
        const double score_of_whites = chessboard.score_of_whites();
        const double score_of_blacks = chessboard.score_of_blacks();
        white_scores.push_back(score_of_whites);
        black_scores.push_back(score_of_blacks);
    }
    // create output file
    // keep only filename parts
    std::vector<std::string> filenames;
    filenames.reserve(num_boards);
    std::ranges::transform(board_paths, std::back_inserter(filenames), [](const std::filesystem::path& board_path) { return board_path.filename().string(); });
    // calculate width of first column
    constexpr std::string_view filename_header = "Chessboard filename";
    size_t filename_column_width = filename_header.length();
    for (const std::string& filename : filenames) {
        filename_column_width = std::max(filename_column_width, filename.length());
    }
    // write out in table format
    std::ofstream ofs("result.txt");
    std::format_to(std::ostream_iterator<char>(ofs), "| {:{}} | White | Black |\n", filename_header, filename_column_width);
    ofs << "| " << std::string(filename_column_width, '-') << " | ----- | ----- |\n";
    for (int i = 0; i < num_boards; i++) {
        std::format_to(std::ostream_iterator<char>(ofs), "| {:{}} | {:<5} | {:<5} |\n", filenames.at(i), filename_column_width, white_scores.at(i), black_scores.at(i));
    }
} catch (const std::exception& e) {
    std::clog << "Exception: " << e.what() << std::endl;
}
