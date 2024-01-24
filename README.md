# MCDM using SVNN (Single Valued Neutrosophic Numbers)

[![Build Status](https://travis-ci.org/Dradwing/BreadcrumbsMulti-Criteria-Decision-Making-using-SVNN.svg?branch=main)](https://travis-ci.org/Dradwing/Multi-Criteria-Decision-Making-using-SVNN)

## Description

This project implements Multi-Criteria Decision Making (MCDM) using Single Valued Neutrosophic Numbers (SVNN). SVNN is a technique for handling uncertainty in decision-making processes.

## Installation
   
### Prerequisites

Before building and running this C++ program, ensure you have the following prerequisites installed:

1. **MSYS2:**
   - Install MSYS2, a software distribution and building platform for Windows.
     - [MSYS2 Installation Guide](https://www.msys2.org/)

2. **GLPK (from MSYS2):**
   - Install the GNU Linear Programming Kit (GLPK) using MSYS2.
     ```bash
     pacman -S mingw-w64-x86_64-glpk
     ```

3. **JSONcpp (from MSYS2):**
   - Install the JSONcpp library using MSYS2.
     ```bash
     pacman -S mingw-w64-x86_64-jsoncpp
     ```

4. **Python 3.12 (from MSYS2):**
   - Install Python 3.12 using MSYS2.
     ```bash
     pacman -S mingw-w64-x86_64-python
     ```

5. **NumPy (using Python pip):**
   - Ensure the NumPy library is installed using Python's package manager.
     ```bash
     pip install numpy
     ```
## File Structure

Ensure the following files are in the same directory:

- `file.cpp`: Your C++ source file.
- `plot3D.py`: Your Python script.
- `input.txt`: Input file for your C++ program.
- `output.txt`: Output file for your C++ program.
- `pso_results.csv`: CSV file required by your Python script.

## Building and Running

After ensuring the prerequisites are met, the final build file should like this:

```json
{
    "cmd": [
        "g++.exe", "-std=c++17", "${file}",
        "-o", "${file_base_name}.exe",
        "-ljsoncpp",
        "-lglpk", 
        "-I", "C:/msys64/ucrt64/include",
        "-I", "C:/Users/90581/AppData/Local/Programs/Python/Python312/include",
        "-I", "C:/Users/90581/AppData/Local/Programs/Python/Python312/Lib/site-packages/numpy/core/include",
        "-L", "C:/Users/90581/AppData/Local/Programs/Python/Python312/libs",
        "-lpython312",
        "-Wno-deprecated-declarations",
        "&&", "${file_base_name}.exe<input.txt>output.txt"
    ],
    "shell": true,
    "working_dir": "$file_path",
    "selector": "source.cpp"
}

```

## Sample Input

### Input.txt

```plaintext

4
6
5
vp
0.05 0.95 0.95
p
0.25 0.75 0.75
g
0.50 0.50 0.50
vg 
0.75 0.25 0.25
ex
0.95 0.05 0.05
4
vg ex vg g g p
vg g g ex vg vg
g ex ex g vg g
ex vg g ex vg vg

vg vg vg g vg p
ex vg vg vg p p 
p ex ex vg g g
g g ex vg ex ex

vg vg ex vg vg g
ex g ex vg ex vg
p ex ex vg g vg
g g vg ex ex ex
ex vp p vg vg vg
g g ex vg g ex
p ex vg g vg vg
vg vg g g vg g
0.5
0.1 0.2
0.1 0.2
0.1 0.25
0.1 0.2
0.1 0.2
0.1 0.2
```
## Sample Output

### Output.txt

```plaintext

Input the number of alternatives:
Input the number of dependent factors:
Input the number of linguistic terms:
Input linguistic term:
Input respective values for truth, indeterminacy and falsity membership:
Input linguistic term:
Input respective values for truth, indeterminacy and falsity membership:
Input linguistic term:
Input respective values for truth, indeterminacy and falsity membership:
Input linguistic term:
Input respective values for truth, indeterminacy and falsity membership:
Input linguistic term:
Input respective values for truth, indeterminacy and falsity membership:

Input the number of experts:
For expert 1

For alternative 1 enter linguistic terms of all factors:

For alternative 2 enter linguistic terms of all factors:

For alternative 3 enter linguistic terms of all factors:

For alternative 4 enter linguistic terms of all factors:

For expert 2

For alternative 1 enter linguistic terms of all factors:

For alternative 2 enter linguistic terms of all factors:

For alternative 3 enter linguistic terms of all factors:

For alternative 4 enter linguistic terms of all factors:

For expert 3

For alternative 1 enter linguistic terms of all factors:

For alternative 2 enter linguistic terms of all factors:

For alternative 3 enter linguistic terms of all factors:

For alternative 4 enter linguistic terms of all factors:

For expert 4

For alternative 1 enter linguistic terms of all factors:

For alternative 2 enter linguistic terms of all factors:

For alternative 3 enter linguistic terms of all factors:

For alternative 4 enter linguistic terms of all factors:


Input the value of alpha: Enter the weight range for 1 factor:
Enter the weight range for 2 factor:
Enter the weight range for 3 factor:
Enter the weight range for 4 factor:
Enter the weight range for 5 factor:
Enter the weight range for 6 factor:
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Simplex Method - Objective Value: 2.60583
Simplex Method - Iterations: 5
Average Execution Time: 30.7 microseconds

Final Weights for each factor: 0.19 0.19 0.24 0.19 0.1 0.09
Optimal sequence is: 4 2 1 3

```
