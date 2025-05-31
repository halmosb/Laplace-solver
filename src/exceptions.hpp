#ifndef _EXCEPTIONS_HPP_
#define _EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>

namespace laplace {

class ParserException : public std::runtime_error {
   public:
    explicit ParserException(const std::string& message)
        : std::runtime_error(message) {}
};

}  // namespace laplace

#endif  // _EXCEPTIONS_HPP_