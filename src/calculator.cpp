#include "calculator.hpp"

#include <cmath>
#include <iostream>

#include "HDF5Writer.hpp"
#include "utilities.hpp"

#ifdef USE_OPENMP
#include <omp.h>
#endif

namespace laplace {

Calculator::Calculator(const ParameterList& plist) : d_plist(plist) {}

void Calculator::jacobi() {
    // Using Jacobi method for solving the differental equation
    Array2D data_curr =
        FileHandler::read(d_plist.initial_state_path, ' ', d_plist);
    Array2D data_prew = data_curr;
    Array2D mask = FileHandler::read(d_plist.mask_path, ' ', d_plist);
    // Iterations
    unsigned num_of_iterations = 0;
    for (num_of_iterations = 0; num_of_iterations < d_plist.max_iter;
         num_of_iterations++) {
        // Copy the current state to be the previous one
        data_prew = data_curr;

        // Update the values using Jacobi method
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < d_plist.N; i++) {
            for (int j = 0; j < d_plist.M; j++) {
                if (mask[i][j] == true) {
                    continue;
                }
                data_curr[i][j] = 0.25f *
                                  (data_prew[i + 1][j] + data_prew[i - 1][j] +
                                   data_prew[i][j + 1] + data_prew[i][j - 1]);
            }
        }
        float max_error = 0;
        #pragma omp parallel for reduction(max : max_error) collapse(2)
        for (int x = 0; x < d_plist.N; ++x) {
            for (int y = 0; y < d_plist.M; ++y) {
                float error = std::fabs(data_curr[x][y] - data_prew[x][y]);
                max_error = std::fmax(max_error, error);
            }
        }
        // If converged
        if (max_error <= d_plist.tol) {
            break;
        }
    }
    d_plist.num_of_iterations = num_of_iterations;

    Utilities::save_print_results(d_plist, data_curr, num_of_iterations);

    return;
}

}  // namespace laplace