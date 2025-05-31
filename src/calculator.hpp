#ifndef _CALCULATOR_HPP_
#define _CALCULATOR_HPP_

#include "filehandler.hpp"
#include "parameterlist.hpp"

namespace laplace {

class Calculator {
   public:
    using Array2D = boost::multi_array<float, 2>;

    Calculator(const ParameterList& plist);
    void jacobi();

   private:
    ParameterList d_plist;
};

}  // namespace laplace

#endif  // _CALCULATOR_HPP_