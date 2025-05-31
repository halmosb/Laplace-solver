#include <gtest/gtest.h>

#include <cstdio>  // for std::remove
#include <fstream>
#include <string>
#include <vector>

#include "exceptions.hpp"
#include "parser.hpp"

using namespace laplace;

class ParserTest : public ::testing::Test {
   protected:
    std::vector<std::string> args;
    std::vector<char*> argv;

    void SetArgs(const std::vector<std::string>& arguments) {
        args = arguments;
        argv.clear();
        for (auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
    }

    Parser CreateParser() {
        return Parser(static_cast<int>(argv.size()), argv.data());
    }
};

TEST_F(ParserTest, HelpMessageReturnsFalse) {
    SetArgs({"program", "--help"});
    Parser parser = CreateParser();

    EXPECT_FALSE(parser.parse());
}

TEST_F(ParserTest, MissingRequiredArgsThrows) {
    SetArgs({"program", "--N", "100"});
    Parser parser = CreateParser();

    EXPECT_THROW({ parser.parse(); }, ParserException);
}

TEST_F(ParserTest, ParsesRequiredArguments) {
    SetArgs({"program", "--N", "128", "--initial-state", "init.txt", "--mask",
             "mask.txt", "--result-path", "results.txt"});
    Parser parser = CreateParser();

    ASSERT_TRUE(parser.parse());

    const auto& params = parser.get_parameter_list();
    EXPECT_EQ(params.N, 128);
    EXPECT_EQ(params.initial_state_path, "init.txt");
    EXPECT_EQ(params.mask_path, "mask.txt");
    EXPECT_EQ(params.result_path, "results.txt");
    EXPECT_FLOAT_EQ(params.tol, 0.001f);
    EXPECT_EQ(params.max_iter, 1000);
    EXPECT_EQ(params.output, 2);
}

TEST_F(ParserTest, ParsesAllArguments) {
    SetArgs({"program", "--N", "64", "--initial-state", "state.dat",
             "--result-path", "result.dat", "--mask", "mask.dat", "--tol",
             "0.0005", "--max-iter", "2000", "--output", "2"});
    Parser parser = CreateParser();

    ASSERT_TRUE(parser.parse());

    const auto& params = parser.get_parameter_list();
    EXPECT_EQ(params.N, 64);
    EXPECT_EQ(params.initial_state_path, "state.dat");
    EXPECT_EQ(params.mask_path, "mask.dat");
    EXPECT_EQ(params.result_path, "result.dat");
    EXPECT_FLOAT_EQ(params.tol, 0.0005f);
    EXPECT_EQ(params.max_iter, 2000);
    EXPECT_EQ(params.output, 2);
}

TEST_F(ParserTest, ParsesFromConfigFile) {
    // Create temporary config file
    const std::string config_filename = "test_config.cfg";
    std::ofstream config(config_filename);
    ASSERT_TRUE(config.is_open());

    config << "N=256\n";
    config << "initial-state=init_from_config.txt\n";
    config << "mask=mask_from_config.txt\n";
    config << "result-path=result_from_config.txt\n";
    config << "tol=0.01\n";
    config << "max-iter=3000\n";
    config << "output=0\n";
    config.close();

    SetArgs({"program", "--config", config_filename});
    Parser parser = CreateParser();

    ASSERT_TRUE(parser.parse());

    const auto& params = parser.get_parameter_list();
    EXPECT_EQ(params.N, 256);
    EXPECT_EQ(params.initial_state_path, "init_from_config.txt");
    EXPECT_EQ(params.mask_path, "mask_from_config.txt");
    EXPECT_EQ(params.result_path, "result_from_config.txt");
    EXPECT_FLOAT_EQ(params.tol, 0.01f);
    EXPECT_EQ(params.max_iter, 3000);
    EXPECT_EQ(params.output, 0);

    std::remove(config_filename.c_str());  // cleanup
}

TEST_F(ParserTest, ConfigFileMissingThrows) {
    SetArgs({"program", "--config", "nonexistent.cfg"});

    Parser parser = CreateParser();
    EXPECT_THROW({ parser.parse(); }, ParserException);
}
