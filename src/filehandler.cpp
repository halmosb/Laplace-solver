#include "filehandler.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace laplace {

FileHandler::Array2D FileHandler::read(const std::string& filename, char delimiter) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open file for reading: " + filename);
    }

    std::vector<std::vector<float>> temp_data;
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string val;
        std::vector<float> row;
        while (std::getline(iss, val, delimiter)) {
            if (!val.empty()) {
                try {
                    row.push_back(std::stof(val));
                } catch (const std::invalid_argument&) {
                    throw std::runtime_error("Invalid numeric value: " + val);
                }
            }
        }
        if (!row.empty()) {
            temp_data.push_back(row);
        }
    }

    size_t rows = temp_data.size();
    if (rows == 0) {
        throw std::runtime_error("Input file is empty.");
    }

    size_t cols = temp_data[0].size();
    for (const auto& row : temp_data) {
        if (row.size() != cols) {
            throw std::runtime_error("Inconsistent row lengths in file.");
        }
    }

    if (rows != cols) {
        throw std::runtime_error("Matrix is not square: " + std::to_string(rows) + " x " + std::to_string(cols));
    }

    Array2D array(boost::extents[rows][cols]);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            array[i][j] = temp_data[i][j];

    return array;
}

void FileHandler::write(const std::string& filename, const Array2D& array, char delimiter) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    size_t rows = array.shape()[0];
    size_t cols = array.shape()[1];

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            outfile << array[i][j];
            if (j < cols - 1) {
                outfile << delimiter;
            }
        }
        outfile << '\n';
    }
}

} // namespace laplace