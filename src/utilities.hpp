#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_

#include <boost/multi_array.hpp>
#include <iostream>

#include "parameterlist.hpp"

using Array2D = boost::multi_array<float, 2>;

namespace laplace {

class Utilities {
   public:
    static void print_matrix(const Array2D& array);
    static bool ends_with(const std::string& value, const std::string& ending);
    static void save_print_results(const ParameterList& plist,
                                   const Array2D& data, int num_of_iterations);
};

}  // namespace laplace

#endif  // LAPACE_UTILITIES_HPP_
