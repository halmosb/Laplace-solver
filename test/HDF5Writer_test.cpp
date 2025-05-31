#include "HDF5Writer.hpp"

#include <H5Cpp.h>
#include <gtest/gtest.h>

#include "parameterlist.hpp"

using namespace laplace;

TEST(HDF5WriterTest, WritesAndReadsHDF5) {
    std::string filename = "test.h5";
    ParameterList plist;
    plist.N = 3;
    plist.M = 3;
    plist.max_iter = 10;
    plist.tol = 1e-2;

    boost::multi_array<float, 2> result(boost::extents[3][3]);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) result[i][j] = i + j;

    HDF5Writer writer(filename);
    writer.writeParameters(plist);
    writer.writeResult(result);
    writer.close();

    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5::DataSet dataset = file.openDataSet("result");
    H5::DataSpace dataspace = dataset.getSpace();
    hsize_t dims[2];
    dataspace.getSimpleExtentDims(dims, nullptr);
    EXPECT_EQ(dims[0], 3);
    EXPECT_EQ(dims[1], 3);
}
