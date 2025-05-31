#include <H5Cpp.h>
#include <gtest/gtest.h>

#include <boost/multi_array.hpp>
#include <filesystem>

#include "HDF5Writer.hpp"

using namespace laplace;
namespace fs = std::filesystem;

class HDF5WriterTest : public ::testing::Test {
   protected:
    std::string testFile = "test_output.h5";

    void TearDown() override {
        if (fs::exists(testFile)) fs::remove(testFile);
    }

    ParameterList createTestParameters() {
        return {
            5,              // N
            5,              // M
            0.001f,         // tol
            50,             // max_iter
            "state.h5",     // initial_state_path
            "mask.h5",      // mask_path
            1,              // output
            "config.yaml",  // config_path
            "results.csv"   // result_path
        };
    }

    boost::multi_array<float, 2> createTestResult(int N) {
        boost::multi_array<float, 2> result(boost::extents[N][N]);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                result[i][j] = static_cast<float>(i + j);
        return result;
    }

    std::string readStringAttr(const H5::Group& group,
                               const std::string& name) {
        H5::StrType strType(H5::PredType::C_S1, H5T_VARIABLE);
        H5::Attribute attr = group.openAttribute(name);
        std::string val;
        attr.read(strType, val);
        return val;
    }
};

TEST_F(HDF5WriterTest, WritesParametersCorrectly) {
    ParameterList params = createTestParameters();
    HDF5Writer writer(testFile);
    writer.writeParameters(params);
    writer.close();

    H5::H5File file(testFile, H5F_ACC_RDONLY);
    H5::Group group = file.openGroup("/parameters");

    H5::Attribute attr = group.openAttribute("N");
    int N;
    attr.read(H5::PredType::NATIVE_INT, &N);
    EXPECT_EQ(N, params.N);

    EXPECT_EQ(readStringAttr(group, "config_path"), params.config_path);
    EXPECT_EQ(readStringAttr(group, "initial_state_path"),
              params.initial_state_path);
}

TEST_F(HDF5WriterTest, WritesResultDatasetCorrectly) {
    ParameterList params = createTestParameters();
    boost::multi_array<float, 2> result = createTestResult(params.N);

    HDF5Writer writer(testFile);
    writer.writeParameters(params);
    writer.writeResult(result);
    writer.close();

    H5::H5File file(testFile, H5F_ACC_RDONLY);
    H5::DataSet dataset = file.openDataSet("result");

    std::vector<float> data(params.N * params.N);
    dataset.read(data.data(), H5::PredType::NATIVE_FLOAT);

    for (int i = 0; i < params.N; ++i)
        for (int j = 0; j < params.N; ++j)
            EXPECT_FLOAT_EQ(data[i * params.N + j], static_cast<float>(i + j));
}

TEST_F(HDF5WriterTest, CreatesExpectedFileStructure) {
    ParameterList params = createTestParameters();
    boost::multi_array<float, 2> result = createTestResult(params.N);

    HDF5Writer writer(testFile);
    writer.writeParameters(params);
    writer.writeResult(result);
    writer.close();

    EXPECT_TRUE(fs::exists(testFile));

    H5::H5File file(testFile, H5F_ACC_RDONLY);
    EXPECT_NO_THROW(file.openGroup("/parameters"));
    EXPECT_NO_THROW(file.openDataSet("result"));
}

TEST_F(HDF5WriterTest, OverwritesFileOnCreation) {
    {
        HDF5Writer writer(testFile);
        writer.writeParameters(createTestParameters());
        writer.close();
    }

    {
        HDF5Writer writer(testFile);  // Should overwrite
        ParameterList params = createTestParameters();
        params.N = 99;  // Different parameter
        writer.writeParameters(params);
        writer.close();
    }

    H5::H5File file(testFile, H5F_ACC_RDONLY);
    H5::Group group = file.openGroup("/parameters");
    int N = -1;
    group.openAttribute("N").read(H5::PredType::NATIVE_INT, &N);
    EXPECT_EQ(N, 99);
}
