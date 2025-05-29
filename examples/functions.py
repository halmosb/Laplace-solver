import numpy as np
import argparse
import configparser

def generate_ex1(N, path):
    data = np.zeros((N, N), dtype=np.float32)
    data[:, 0] = 1
    np.savetxt(f"{path}/data_N{N}.csv", data, delimiter=" ")

    mask = np.zeros_like(data)
    mask[:, 0] = 1
    mask[:, -1] = 1
    mask[0, :] = 1
    mask[-1, :] = 1
    np.savetxt(f"{path}/mask_N{N}.csv", mask, delimiter=" ")

def parse():
    # First, parse just the --config argument to load defaults
    config_parser = argparse.ArgumentParser(add_help=False)
    config_parser.add_argument('--config', type=str, help='Path to the config file')
    config_args, remaining_argv = config_parser.parse_known_args()

    # Load config defaults if a config file was provided
    defaults = {}
    if config_args.config:
        config = configparser.ConfigParser()
        with open(config_args.config) as stream:
            config.read_string("[DEFAULT]\n" + stream.read()) 
            defaults.update(config['DEFAULT'])

    # Main parser with optional defaults
    parser = argparse.ArgumentParser(
        parents=[config_parser],
        description='Your Program Description'
    )
    parser.set_defaults(**defaults)

    parser.add_argument('--N', type=int, required='n' not in defaults, default=5 if "n" not in defaults else defaults["n"], help='Size of the grid')
    parser.add_argument('--initial-state', type=str, required='initial-state' not in defaults, default="" if "initial-state" not in defaults else defaults["initial-state"], help='Path to the initial state file')
    parser.add_argument('--mask', type=str, required='mask' not in defaults, default="" if "mask" not in defaults else defaults["mask"], help='Path to the mask file')
    parser.add_argument('--result-path', type=str, required='result-path' not in defaults, default="" if "result-path" not in defaults else defaults["result-path"], help='Path to save the results')
    parser.add_argument('--tol', type=float, default=0.001 if "tol" not in defaults else defaults["tol"], help='Tolerance')
    parser.add_argument('--max-iter', type=int, default=1000 if "max-iter" not in defaults else defaults["max-iter"], help='Maximum number of iterations')
    parser.add_argument('--output', type=int, default=2 if "output" not in defaults else defaults["output"], help='Amount of information printed')
    parser.add_argument('--config2', type=str, default=config_args.config, help=argparse.SUPPRESS)

    args = parser.parse_args(remaining_argv)
    return args