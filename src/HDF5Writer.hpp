#ifndef _HDF5WRITER_HPP_
#define _HDF5WRITER_HPP_

#include <H5Cpp.h>

#include <boost/multi_array.hpp>
#include <string>

#include "parameterlist.hpp"

namespace laplace {

class HDF5Writer {
   public:
    HDF5Writer(const std::string& filename);
    void writeParameters(const ParameterList& params);
    void writeResult(const boost::multi_array<float, 2>& result);
    void close();

   private:
    H5::H5File file_;
    H5::Group paramGroup_;

    void writeAttribute(const std::string& name, int value);
    void writeAttribute(const std::string& name, float value);
    void writeAttribute(const std::string& name, const std::string& value);
};

}  // namespace laplace

#endif  // HDF5WRITER_H
