#include "calculator.hpp"

#include <gtest/gtest.h>

#include <boost/multi_array.hpp>
#include <fstream>

#include "filehandler.hpp"
#include "parameterlist.hpp"

using namespace laplace;

TEST(CalculatorTest, ConvergesOnKnownGrid) {
    ParameterList plist;
    plist.N = 5;
    plist.M = 5;
    plist.tol = 1e-3;
    plist.max_iter = 500;
    plist.initial_state_path = "init.csv";
    plist.mask_path = "mask.csv";
    plist.result_path = "result.csv";
    plist.output = 0;

    // Write simple input files
    std::ofstream("init.csv")
        << "0 0 0 0 0\n0 0 0 0 0\n0 0 0 0 0\n0 0 0 0 0\n0 0 0 0 0";
    std::ofstream("mask.csv")
        << "1 1 1 1 1\n1 0 0 0 1\n1 0 0 0 1\n1 0 0 0 1\n1 1 1 1 1";

    Calculator calc(plist);
    calc.jacobi();

    EXPECT_LT(plist.num_of_iterations, plist.max_iter);
}

#ifdef USE_OPENMP
TEST(CalculatorTest, OpenMPParallelismEffect) {
    ParameterList plist;
    plist.N = 200;
    plist.M = 200;
    plist.tol = 1e-3;
    plist.max_iter = 200;
    plist.initial_state_path = "init_large.csv";
    plist.mask_path = "mask_large.csv";
    plist.result_path = "result_large.csv";
    plist.output = 0;

    std::ofstream init("init_large.csv");
    std::ofstream mask("mask_large.csv");
    for (int i = 0; i < plist.N; ++i) {
        for (int j = 0; j < plist.M; ++j) {
            init << 0 << (j < plist.M - 1 ? " " : "\n");
            mask << ((i == 0 || j == 0 || i == plist.N - 1 || j == plist.M - 1)
                         ? 1
                         : 0);
            if (j < plist.M - 1)
                mask << " ";
            else
                mask << "\n";
        }
    }

    Calculator calc(plist);
    double start = omp_get_wtime();
    calc.jacobi();
    double end = omp_get_wtime();

    std::cout << "Jacobi with OpenMP took " << (end - start) << "s.\n";
    EXPECT_LT(plist.num_of_iterations, plist.max_iter);
}
#endif
