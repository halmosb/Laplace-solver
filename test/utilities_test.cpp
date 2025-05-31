#include "utilities.hpp"

#include <gtest/gtest.h>

using namespace laplace;

TEST(UtilitiesTest, EndsWithTrue) {
    EXPECT_TRUE(Utilities::ends_with("file.csv", ".csv"));
}

TEST(UtilitiesTest, EndsWithFalse) {
    EXPECT_FALSE(Utilities::ends_with("file.txt", ".csv"));
}
