#include "utilities.hpp"

#include "HDF5Writer.hpp"
#include "filehandler.hpp"

namespace laplace {

void Utilities::print_matrix(const Array2D& array) {
    size_t rows = array.shape()[0];
    size_t cols = array.shape()[1];

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            std::cout << array[i][j];
            if (j < cols - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }
}

bool Utilities::ends_with(const std::string& value, const std::string& ending) {
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void Utilities::save_print_results(const ParameterList& plist,
                                   const Array2D& data, int num_of_iterations) {
    if (plist.output >= 2) {
        std::cout << "Calculation finished after " << num_of_iterations
                  << " iterations, the result is:" << std::endl;
        Utilities::print_matrix(data);
        std::cout << "With shape: (" << data.shape()[0] << ", "
                  << data.shape()[1] << ")" << std::endl;
    }

    if (Utilities::ends_with(plist.result_path, ".csv")) {
        FileHandler::write(plist.result_path, data, ' ');
    } else if (Utilities::ends_with(plist.result_path, ".h5")) {
        // Write data to h5 file
        HDF5Writer writer(plist.result_path);
        writer.writeParameters(plist);
        writer.writeResult(data);
        writer.close();
    } else {
        FileHandler::write(plist.result_path + ".csv", data, ' ');
        // Write data to h5 file
        HDF5Writer writer(plist.result_path + ".h5");
        writer.writeParameters(plist);
        writer.writeResult(data);
        writer.close();
    }

    if (plist.output >= 1) {
        std::cout << "Results saved to file: " << plist.result_path << ", using " << num_of_iterations << " iterations."
                  << std::endl
                  << "bye" << std::endl;
    }
}

}  // namespace laplace
