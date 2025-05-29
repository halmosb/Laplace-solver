#include <iostream>
#include "parser.hpp"
#include "calculator.hpp"


using namespace laplace;
using Array2D = boost::multi_array<float, 2>;

int main(int argc, char* argv[]) {
    Parser parser(argc, argv);
    if (!parser.parse()) {
        return 0;
    }
    ParameterList args = parser.get_parameter_list();

    if (args.output >= 2) {
        std::cout << "Laplace equation solver" << std::endl <<
            "The used parameters are:" << std::endl <<
            "\tN: " << args.N << std::endl <<
            "\ttol: " << args.tol << std::endl <<
            "\tmax iter: " << args.max_iter << std::endl <<
            "\tinitial state path: " << args.initial_state_path << std::endl <<
            "\tmask path: " << args.mask_path << std::endl <<
            "\tresult path: " << args.result_path << std::endl <<
            "\toutput: " << args.output << std::endl <<
            "\tconfig path: " << args.config_path << std::endl;
    }

    Calculator calc(args);
    calc.jacobi();

    return 0;
}

