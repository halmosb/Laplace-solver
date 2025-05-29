import numpy as np
import copy
from functions import *

def jacobi(data, mask, tol=1e-3, max_iter=1000):
    """
    Solves the differential equation using Jacobi
    update scheme.
    data is the initial values,
    mask masks the boundary conditions with 1,
    tol and max_iter determins when the iterations 
    can be stoped.
    Returns the solution and the number of iterations.
    """

    # Copy the initial values
    Psi_curr = copy.deepcopy(data)
    # The iterations
    for n in range(max_iter):
        # The previous values
        Psi_prev = copy.deepcopy(Psi_curr)
        # Rows and columns
        for i in range(data.shape[0]):
            for j in range(data.shape[1]):
                # mask
                if mask[i, j]==1:
                    continue
                # Update values
                Psi_curr[i, j] = 1/4*(
                            Psi_prev[i+1, j]+
                            Psi_prev[i-1, j]+
                            Psi_prev[i, j+1]+
                            Psi_prev[i, j-1]
                        )
        # adding errors and checnking for convergence
        if np.max(np.abs(Psi_prev-Psi_curr)) <= tol:
            break
    return Psi_curr, n


if __name__ == "__main__":
    args = parse()
    N = args.N
    tol = args.tol
    max_iter = args.max_iter
    initial_state_path = args.initial_state
    mask_path = args.mask
    result_path = args.result_path
    output = args.output
    config_path = args.config2

    if output >= 2:
        print(f"Laplace equation solver\nThe used parameters are:")
        print(f"\tN: {N}\n\ttol: {tol}")
        print(f"\tmax iter: {max_iter}\n\tinitial state path: {initial_state_path}")
        print(f"\tresult path: {result_path}")
        print(f"\tmask path: {mask_path}\n\toutput: {output}")
        print(f"\tconfig path: {config_path}")

    data = np.loadtxt(initial_state_path, delimiter=" ")
    mask = np.loadtxt(mask_path, delimiter=" ")

    # Calulating
    result, iterations = jacobi(data=data, mask=mask, tol=tol, max_iter=max_iter)

    if output >= 2:
        print(f"Calulation finished after {iterations} iterations, the result is:")
        print(result)
        print("With shape:", result.shape)
    
    np.savetxt(f"{result_path}", result, delimiter=" ")
    if output >= 1:
        print(f"Results saved to file: {result_path}.")
        print("bye")

