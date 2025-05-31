#ifndef _PARAMETERLIST_HPP_
#define _PARAMETERLIST_HPP_

#include <string>

namespace laplace {

struct ParameterList {
    int N;
    int M;
    float tol;
    int max_iter;
    std::string initial_state_path;
    std::string mask_path;
    int output;
    std::string config_path;
    std::string result_path;
    int num_of_iterations = -1;
};

}  // namespace laplace

#endif  // _PARAMETERLIST_HPP_