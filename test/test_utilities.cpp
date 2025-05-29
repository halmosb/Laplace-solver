#include "utilities.hpp"
#include <gtest/gtest.h>
#include <sstream>

using namespace laplace;

class UtilitiesTest : public ::testing::Test {
protected:
    std::streambuf* original_buf;
    std::ostringstream test_stream;

    void SetUp() override {
        original_buf = std::cout.rdbuf(); // save original buffer
        std::cout.rdbuf(test_stream.rdbuf()); // redirect cout
    }

    void TearDown() override {
        std::cout.rdbuf(original_buf); // restore cout
    }
};

TEST_F(UtilitiesTest, PrintMatrix_2x2) {
    Array2D arr(boost::extents[2][2]);
    arr[0][0] = 1.0f; arr[0][1] = 2.0f;
    arr[1][0] = 3.0f; arr[1][1] = 4.0f;

    Utilities::print_matrix(arr);

    std::string expected = "1, 2\n3, 4\n";
    std::string output = test_stream.str();
    //std::replace(output.begin(), output.end(), '\r', ''); // for Windows compatibility
    EXPECT_EQ(output, expected);
}

TEST_F(UtilitiesTest, PrintMatrix_EmptyMatrix) {
    Array2D arr(boost::extents[0][0]);

    Utilities::print_matrix(arr);

    EXPECT_EQ(test_stream.str(), "");
}

TEST_F(UtilitiesTest, PrintMatrix_1x3) {
    Array2D arr(boost::extents[1][3]);
    arr[0][0] = 0.1f; arr[0][1] = 0.2f; arr[0][2] = 0.3f;

    Utilities::print_matrix(arr);

    std::string expected = "0.1, 0.2, 0.3\n";
    std::string output = test_stream.str();
    //std::replace(output.begin(), output.end(), '\r', ''); // for Windows
    EXPECT_EQ(output, expected);
}
