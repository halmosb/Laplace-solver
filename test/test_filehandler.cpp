#include "filehandler.hpp"
#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>

using namespace laplace;

class FileHandlerTest : public ::testing::Test {
protected:
    std::string test_file = "test_matrix.txt";

    void writeToFile(const std::string& content) {
        std::ofstream out(test_file);
        out << content;
    }

    void TearDown() override {
        std::remove(test_file.c_str());
    }
};

TEST_F(FileHandlerTest, ValidReadAndWrite) {
    std::string content = "1.0 2.0 3.0\n4.0 5.0 6.0\n7.0 8.0 9.0\n";
    writeToFile(content);

    auto matrix = FileHandler::read(test_file);
    EXPECT_EQ(matrix.shape()[0], 3);
    EXPECT_EQ(matrix.shape()[1], 3);
    EXPECT_FLOAT_EQ(matrix[1][1], 5.0f);

    std::string out_file = "output_matrix.txt";
    FileHandler::write(out_file, matrix);
    std::ifstream check(out_file);
    ASSERT_TRUE(check.is_open());
    std::string line;
    std::getline(check, line);
    EXPECT_EQ(line, "1 2 3");
    check.close();
    std::remove(out_file.c_str());
}

TEST_F(FileHandlerTest, InvalidFileReadThrows) {
    EXPECT_THROW(FileHandler::read("non_existent.txt"), std::runtime_error);
}

TEST_F(FileHandlerTest, InconsistentRowLengthsThrows) {
    std::string content = "1.0 2.0\n3.0 4.0 5.0\n";
    writeToFile(content);
    EXPECT_THROW(FileHandler::read(test_file), std::runtime_error);
}

TEST_F(FileHandlerTest, NonSquareMatrixThrows) {
    std::string content = "1.0 2.0 3.0\n4.0 5.0 6.0\n";
    writeToFile(content);
    EXPECT_THROW(FileHandler::read(test_file), std::runtime_error);
}

TEST_F(FileHandlerTest, InvalidNumberThrows) {
    std::string content = "1.0 2.0 a\n4.0 5.0 6.0\n7.0 8.0 9.0\n";
    writeToFile(content);
    EXPECT_THROW(FileHandler::read(test_file), std::runtime_error);
}

TEST_F(FileHandlerTest, EmptyFileThrows) {
    writeToFile("");
    EXPECT_THROW(FileHandler::read(test_file), std::runtime_error);
}
