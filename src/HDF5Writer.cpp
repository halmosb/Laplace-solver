#include "HDF5Writer.hpp"

namespace laplace {

HDF5Writer::HDF5Writer(const std::string& filename)
    : file_(filename, H5F_ACC_TRUNC) {
    paramGroup_ = file_.createGroup("/parameters");
}

void HDF5Writer::writeParameters(const ParameterList& params) {
    writeAttribute("N", params.N);
    writeAttribute("M", params.M);
    writeAttribute("tol", params.tol);
    writeAttribute("max_iter", params.max_iter);
    writeAttribute("initial_state_path", params.initial_state_path);
    writeAttribute("mask_path", params.mask_path);
    writeAttribute("output", params.output);
    writeAttribute("config_path", params.config_path);
    writeAttribute("result_path", params.result_path);
    writeAttribute("num_of_itearations", params.num_of_iterations);
}

void HDF5Writer::writeResult(const boost::multi_array<float, 2>& result) {
    const hsize_t dims[2] = {result.shape()[0], result.shape()[1]};
    H5::DataSpace dataspace(2, dims);

    H5::DataSet dataset =
        file_.createDataSet("result", H5::PredType::NATIVE_FLOAT, dataspace);
    dataset.write(result.data(), H5::PredType::NATIVE_FLOAT);
}

void HDF5Writer::close() {
    file_.flush(H5F_SCOPE_GLOBAL);
    file_.close();
}

// --- Attribute Helpers ---

void HDF5Writer::writeAttribute(const std::string& name, int value) {
    H5::DataSpace dataspace(H5S_SCALAR);
    H5::Attribute attr =
        paramGroup_.createAttribute(name, H5::PredType::NATIVE_INT, dataspace);
    attr.write(H5::PredType::NATIVE_INT, &value);
}

void HDF5Writer::writeAttribute(const std::string& name, float value) {
    H5::DataSpace dataspace(H5S_SCALAR);
    H5::Attribute attr = paramGroup_.createAttribute(
        name, H5::PredType::NATIVE_FLOAT, dataspace);
    attr.write(H5::PredType::NATIVE_FLOAT, &value);
}

void HDF5Writer::writeAttribute(const std::string& name,
                                const std::string& value) {
    H5::StrType strType(H5::PredType::C_S1, H5T_VARIABLE);
    H5::DataSpace dataspace(H5S_SCALAR);
    H5::Attribute attr = paramGroup_.createAttribute(name, strType, dataspace);
    attr.write(strType, value);
}

}  // namespace laplace