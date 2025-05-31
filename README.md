# 🧪 Laplace Solver using Jacobi Iteration

A parallelized C++ implementation of a **Laplace equation solver** using the **Jacobi iterative method**. This solver supports **OpenMP acceleration**, **CSV input/output**, and **HDF5 result storage**, and includes a flexible command-line argument parser with configuration file support.

## 📘 Problem Description

The project solves the **Laplace equation** on a 2D rectangular domain:

$\nabla²u(x, y)=0$

This partial differential equation describes the steady-state distribution of potential fields (e.g., heat, electric potential) over space. The solution is computed using the **Jacobi iterative method**, a classical algorithm for finding approximate solutions to discretized linear systems.

### 💡 Jacobi Iteration Overview

The domain is discretized on an `N × M` grid. At each iteration, the value at every non-fixed (non-boundary) point is updated using the average of its four neighbors:

```
u_new[i][j] = 0.25 * (u_old[i+1][j] + u_old[i-1][j] + u_old[i][j+1] + u_old[i][j-1])
```

This process repeats until either:

* The **residual** (max difference between successive iterations) is less than a tolerance `tol`, or
* A maximum number of iterations `max_iter` is reached.

---

## 🚀 Getting Started

### 📅 Clone the Repository

```bash
git clone https://github.com/halmosb/Laplace-solver
cd Laplace-solver
```

---

## 🛠 Build & Install

This project uses **CMake** for configuration and supports **Google Test** for unit testing.

### 🔧 Prerequisites

* C++17 compiler (e.g., `g++`, `clang++`)
* CMake ≥ 3.12
* [Boost.Program\_options](https://www.boost.org/doc/libs/release/doc/html/program_options.html)
* [HDF5 C++ API](https://www.hdfgroup.org/)
* [Google Test](https://github.com/google/googletest)
* OpenMP

### ⚙️ Build Instructions

```bash
mkdir build
cd build -DCMAKE_BUILD_TYPE=Release
cmake .. 
cmake --build . -j4
```

This builds the main executable `laplace_solver` and test binaries in the `build/` directory.

---

## ⚡ OpenMP Acceleration

This project supports **multithreading** using OpenMP. If available, the Jacobi update loop will run in parallel across multiple CPU cores.

To enable OpenMP (if not enabled by default):

```bash
cmake -DUSE_OPENMP=ON ..
make
```

Control the number of threads via the environment variable:

```bash
export OMP_NUM_THREADS=4
```

---

## 🧪 Run the Solver

You can run the solver using command-line arguments or a configuration file.

### ✅ Required Files

You must provide:

* **Initial state** file: CSV file with initial guess (grid of floats)
* **Mask** file: CSV file of same size with binary flags (1 for fixed/boundary, 0 for updatable points)

### ⚙️ Command-Line Options

```
Generic options:
  -h [ --help ]               Show the help message
  --config arg                Path to the config file

System options:
  --N arg                     Width of the grid
  --M arg                     Height of the grid
  --initial-state arg         Path to the initial state file
  --mask arg                  Path to the mask file
  --result-path arg           Path to save the results file
  --tol arg (=0.001)          Convergence tolerance
  --max-iter arg (=1000)      Maximum number of iterations
  --output arg (=2)           Verbosity level (0 = silent, 1 = result only, 2 = debug)
```

### 🔧 Example Usage

```bash
./laplace_solver --N 100 --M 100 \
                 --initial-state data/init.csv \
                 --mask data/mask.csv \
                 --result-path results/result.csv \
                 --tol 1e-4 --max-iter 500 --output 2
```

Or using a config file:

```ini
# Content of the config.txt file:
N=100
M=100
initial-state=data/init.csv
mask=data/mask.csv
result-path=results/result.csv
tol=0.0001
max-iter=500
output=1
```

Run with:

```bash
./laplace_solver --config config.txt
```

---

## 📄 Output

* **Result CSV**: Final solution grid written to `--result-path`
* **HDF5 Output** (if enabled): Also saves result and metadata to `laplace_result.h5`

---

## ✅ Testing

Run unit tests with:

```bash
ctest --output-on-failure
```

Or individually:

```bash
./calculator_test
./filehandler_test
./utilities_test
```

---

## 📁 Project Structure

```
.
├── include/           # Header files
├── src/               # Source code
├── tests/             # Unit tests (Google Test)
├── data/              # Sample input files
├── build/             # CMake build output
└── CMakeLists.txt     # Build configuration
```

---

## 🧑‍💻 Authors

Developed as part of a C++ numerical methods project. Contributions and suggestions are welcome.

---

## 📄 License

This project is licensed under the MIT License. See `LICENSE` for details.
