#ifndef _FILE_HANDLER_HPP_
#define _FILE_HANDLER_HPP_

#include <boost/multi_array.hpp>
#include <string>

#include "parameterlist.hpp"

namespace laplace {

class FileHandler {
   public:
    using Array2D = boost::multi_array<float, 2>;

    static Array2D read(const std::string& filename, char delimiter,
                        const ParameterList& plist);
    static void write(const std::string& filename, const Array2D& array,
                      char delimiter = ' ');
};

}  // namespace laplace

#endif  // FILE_HANDLER_HPP
