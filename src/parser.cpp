#include "parser.hpp"

#include <fstream>
#include <iostream>

#include "exceptions.hpp"

namespace laplace {
Parser::Parser(int argc, char** argv)
    : generic_options("Generic options"),
      system_options("System options"),
      d_plist() {
    d_argv = argv;
    d_argc = argc;

    generic_options.add_options()("help,h", "Show the help message")(
        "config", po::value<std::string>(&d_plist.config_path),
        "Path to the config file");

    system_options.add_options()("N", po::value<int>(&d_plist.N)->required(),
                                 "Width of the grid")(
        "M", po::value<int>(&d_plist.M)->required(), "Height of the grid")(
        "initial-state",
        po::value<std::string>(&d_plist.initial_state_path)->required(),
        "Path to the initial state file")(
        "mask", po::value<std::string>(&d_plist.mask_path)->required(),
        "Path to the mask file")(
        "result-path", po::value<std::string>(&d_plist.result_path)->required(),
        "Path to save the results file")(
        "tol", po::value<float>(&d_plist.tol)->default_value(0.001),
        "Tolerance")("max-iter",
                     po::value<int>(&d_plist.max_iter)->default_value(1000),
                     "Maximum number of iterations")(
        "output", po::value<int>(&d_plist.output)->default_value(2),
        "Amount of information printed");

    cmdline_options.add(generic_options).add(system_options);
    config_file_options.add(generic_options).add(system_options);
}

bool Parser::parse() {
    try {
        po::store(po::parse_command_line(d_argc, d_argv, cmdline_options), vm);

        if (vm.count("help")) {
            std::cout << cmdline_options << std::endl;
            return false;
        }

        if (vm.count("config")) {
            std::string config_path = vm["config"].as<std::string>();
            std::ifstream config_file(config_path.c_str());

            if (!config_file) {
                throw ParserException("Error: Cannot open config file: " +
                                      config_path);
            }

            po::store(po::parse_config_file(config_file, config_file_options),
                      vm);
        }

        po::notify(vm);
    } catch (const std::exception& e) {
        std::stringstream ss;
        ss << "Command-line parse error: " << e.what() << "\n"
           << cmdline_options << "\n";
        throw ParserException(ss.str());
    }

    return true;
}

}  // namespace laplace
