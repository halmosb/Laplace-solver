#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <boost/program_options.hpp>

#include "parameterlist.hpp"

namespace po = boost::program_options;

namespace laplace {
class Parser {
   public:
    Parser(int argc, char** argv);
    bool parse();
    ParameterList get_parameter_list() { return d_plist; };

    // ParameterList& getParameterList() { return d_plist; };

   private:
    po::options_description generic_options;
    po::options_description system_options;
    po::options_description cmdline_options;
    po::options_description config_file_options;

    po::variables_map vm;
    int d_argc;
    char** d_argv;

    ParameterList d_plist;
};
}  // namespace laplace

#endif  // _PARSER_HPP_