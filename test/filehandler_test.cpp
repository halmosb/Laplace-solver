#include "filehandler.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include "parameterlist.hpp"

using namespace laplace;

TEST(FileHandlerTest, ThrowsOnNonExistentFile) {
    ParameterList plist;
    plist.N = 3;
    plist.M = 3;
    EXPECT_THROW(FileHandler::read("no_such_file.csv", ' ', plist),
                 std::runtime_error);
}

TEST(FileHandlerTest, ReadsAndWritesCSVCorrectly) {
    ParameterList plist;
    plist.N = 2;
    plist.M = 2;

    std::string testfile = "temp.csv";
    std::ofstream(testfile) << "1 2\n3 4";

    auto array = FileHandler::read(testfile, ' ', plist);
    EXPECT_EQ(array[0][0], 1);
    EXPECT_EQ(array[1][1], 4);

    FileHandler::write("output.csv", array, ' ');
    std::ifstream result("output.csv");
    std::string line;
    std::getline(result, line);
    EXPECT_EQ(line, "1 2");
}
