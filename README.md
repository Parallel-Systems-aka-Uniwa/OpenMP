<p align="center">
  <img src="https://www.especial.gr/wp-content/uploads/2019/03/panepisthmio-dut-attikhs.png" alt="UNIWA" width="150"/>
</p>

<p align="center">
  <strong>UNIVERSITY OF WEST ATTICA</strong><br>
  SCHOOL OF ENGINEERING<br>
  DEPARTMENT OF COMPUTER ENGINEERING AND INFORMATICS
</p>

---

<p align="center">
  <strong>Parallel Systems</strong>
</p>

<h1 align="center">
  Parallel Computing using OpenMP
</h1>

<p align="center">
  <strong>Vasileios Evangelos Athanasiou</strong><br>
  Student ID: 19390005
</p>

<p align="center">
  <a href="https://github.com/Ath21" target="_blank">GitHub</a> ·
  <a href="https://www.linkedin.com/in/vasilis-athanasiou-7036b53a4/" target="_blank">LinkedIn</a>
</p>

<hr/>

<p align="center">
  <strong>Supervision</strong>
</p>

<p align="center">
  Supervisor: Vasileios Mamalis, Professor
</p>
<p align="center">
  <a href="https://ice.uniwa.gr/en/emd_person/vassilios-mamalis/" target="_blank">UNIWA Profile</a>
</p>


<p align="center">
  Co-supervisor: Michalis Iordanakis, Special Technical Laboratory Staff
</p>

<p align="center">
  <a href="https://scholar.google.com/citations?user=LiVuwVEAAAAJ&hl=en" target="_blank">UNIWA Profile</a>
</p>

</hr>

<p align="center">
  Athens, December 2024
</p>


---

# Parallel Computing using OpenMP

This repository provides an overview of a parallel computing project using **OpenMP**. The project focuses on **matrix manipulation**, **diagonal dominance verification**, and **parallel reduction techniques**.

---

## Table of Contents

| Section | Folder/File | Description |
|------:|-------------|-------------|
| 1 | `assign/` | Assignment material for the OpenMP workshop |
| 1.1 | `assign/_Par_Sys_Ask_1_2024-25.pdf` | Assignment description in English |
| 1.2 | `assign/_Παρ_Συσ_Ασκ_1_2024-25.pdf` | Assignment description in Greek |
| 2 | `docs/` | Documentation and performance data for OpenMP parallel computing |
| 2.1 | `docs/Parallel-Computing-using-OpenMP.pdf` | English documentation for OpenMP parallel computing |
| 2.2 | `docs/Παράλληλος-Υπολογισμός-με-OpenMP.pdf` | Greek documentation for OpenMP parallel computing |
| 2.3 | `docs/Times.xlsx` | Performance timing results |
| 3 | `src/` | Source code and input/output data for OpenMP exercises |
| 3.1 | `src/omp.c` | Main OpenMP program |
| 3.2 | `src/A/` | Input data files for exercise A |
| 3.2.1 | `src/A/A_no_strict_diagonial.txt` | Special test case for A |
| 3.2.2 | `src/A/A_T1_N10_CZ2.txt` | Input for 1 thread, N=10, CZ=2 |
| 3.2.3 | `src/A/A_T1_N16_CZ3.txt` | Input for 1 thread, N=16, CZ=3 |
| 3.2.4 | `src/A/A_T1_N25_CZ5.txt` | Input for 1 thread, N=25, CZ=5 |
| 3.2.5 | `src/A/A_T1_N400_CZ20.txt` | Input for 1 thread, N=400, CZ=20 |
| 3.2.6 | `src/A/A_T1_N1000_CZ100.txt` | Input for 1 thread, N=1000, CZ=100 |
| 3.2.7 | `src/A/A_T12_N10_CZ2.txt` | Input for 12 threads, N=10, CZ=2 |
| 3.2.8 | `src/A/A_T12_N16_CZ3.txt` | Input for 12 threads, N=16, CZ=3 |
| 3.2.9 | `src/A/A_T12_N25_CZ5.txt` | Input for 12 threads, N=25, CZ=5 |
| 3.2.10 | `src/A/A_T12_N400_CZ20.txt` | Input for 12 threads, N=400, CZ=20 |
| 3.2.11 | `src/A/A_T12_N1000_CZ100.txt` | Input for 12 threads, N=1000, CZ=100 |
| 3.2.12 | `src/A/A_T2_N10_CZ2.txt` | Input for 2 threads, N=10, CZ=2 |
| 3.2.13 | `src/A/A_T2_N16_CZ3.txt` | Input for 2 threads, N=16, CZ=3 |
| 3.2.14 | `src/A/A_T2_N25_CZ5.txt` | Input for 2 threads, N=25, CZ=5 |
| 3.2.15 | `src/A/A_T2_N400_CZ20.txt` | Input for 2 threads, N=400, CZ=20 |
| 3.2.16 | `src/A/A_T2_N1000_CZ100.txt` | Input for 2 threads, N=1000, CZ=100 |
| 3.2.17 | `src/A/A_T4_N10_CZ2.txt` | Input for 4 threads, N=10, CZ=2 |
| 3.2.18 | `src/A/A_T4_N16_CZ4.txt` | Input for 4 threads, N=16, CZ=4 |
| 3.2.19 | `src/A/A_T4_N25_CZ5.txt` | Input for 4 threads, N=25, CZ=5 |
| 3.2.20 | `src/A/A_T4_N400_CZ20.txt` | Input for 4 threads, N=400, CZ=20 |
| 3.2.21 | `src/A/A_T8_N10_CZ2.txt` | Input for 8 threads, N=10, CZ=2 |
| 3.2.22 | `src/A/A_T8_N16_CZ3.txt` | Input for 8 threads, N=16, CZ=3 |
| 3.2.23 | `src/A/A_T8_N25_CZ5.txt` | Input for 8 threads, N=25, CZ=5 |
| 3.2.24 | `src/A/A_T8_N400_CZ20.txt` | Input for 8 threads, N=400, CZ=20 |
| 3.3 | `src/B/` | Input data files for exercise B (same pattern as A) |
| 3.4 | `src/Output/` | Output result files from OpenMP execution |
| 4 | `README.md` | Repository overview and usage instructions |

---

## Project Overview

This project evaluates the performance and implementation of a multithreaded program using the **fork-join parallel execution model**. Key highlights:

- High-level **OpenMP techniques**
- Task division across threads
- Synchronized communication
- Performance scaling and speedup measurement between sequential and parallel execution

---

## Problem Description

The software performs operations on square matrices \(N \times N\):

### Task A: Diagonal Dominance Check
Check if matrix **A** is strictly diagonally dominant:

$$
|A_{ii}| > \sum_{j=0, j \neq i}^{N-1} |A_{ij}|
$$

### Task B: Maximum Diagonal Element
Find the maximum absolute element \(m\) on the diagonal of **A**.

### Task C: Generate Matrix B
Create matrix **B** based on \(m\):

$$
B_{ij} =
\begin{cases}
m - |A_{ij}| & i \neq j \\
m & i = j
\end{cases}
$$

### Task D: Minimum Element in B
Compute the minimum absolute value in **B** using three methods:

1. **OpenMP reduction directive**
2. **Critical region protection**
3. **Binary Tree Algorithm for parallel reduction**

---

## Implementation Details

### Data Structures

- **A, B**: Dynamic 2D arrays storing matrix data
- **M**: Dynamic 1D array for the binary tree reduction
- **chunk**: Number of iterations per thread (parameter **CZ**)

### Binary Tree Algorithm
Manually replaces the OpenMP reduction. Threads synchronize in phases, comparing pairs until a single minimum remains in the first position.

### Parallel Directives

- `#pragma omp parallel` → Activates threads
- `#pragma omp for schedule(static, chunk)` → Distributes loop iterations
- `#pragma omp atomic` → Ensures atomic updates (e.g., dominance flag)
- `reduction(max: m)` → Handles maximum calculation automatically

---

## Performance Analysis

- Execution times are measured to calculate **Speedup (S)**:

$$
S = \frac{T_{sequential}}{T_{parallel}}
$$

### Test Parameters

- **Matrix Size (N)**: 10×10 up to 10,000×10,000
- **Thread Count (T)**: 1 to 12 threads
- **Chunk Size (CZ)**: Number of elements per thread

---

## 5. Usage Instructions

1. **Generate Matrix**  
   Matrices can be strictly diagonally dominant or random based on a seed.

2. **Execute Tasks**  
   Program executes **Tasks A–D** using parallel regions for each.

3. **Results**  
   Output saved to text files (e.g., `Output_T8_N400_CZ20.txt`) for verification and analysis.

---

# Installation & Setup Guide

This repository contains laboratory simulations and source code for **Parallel Computing using OpenMP**, developed as part of the **Parallel Systems course** at the **University of West Attica**. The focus is on **matrix operations, diagonal dominance checking, and parallel reduction algorithms** using the **fork-join model**.

All exercises are implemented in **C** using **OpenMP**.

---

## Prerequisites

### Required Software
- **GCC Compiler** (version supporting OpenMP, typically ≥ 4.2)
- **OpenMP library support**
- Operating System: Linux, macOS, or Windows (via WSL or MinGW)

### Optional Software
- **Text Editor / IDE** for source code editing (VSCode, CLion, etc.)
- **Spreadsheet Viewer** for analyzing timing results (`docs/Times.xlsx`)

---

## Installation Steps

### 1. Clone the Repository

Clone using Git:

```bash
git clone https://github.com/Parallel-Systems-aka-Uniwa/OpenMP.git
```

Alternatively, download the ZIP file and extract it locally.

### 2. Navigate to Project Directory
```bash
cd OpenMP
```

Ensure the following structure exists:
```bash
assign/
docs/
src/
README.md
```

`src/` contains the main program (omp.c) and input files for exercises A and B.

`docs/` contains documentation and performance results.

---

## Compilation Instructions
Compile the OpenMP C program using GCC:
```bash
gcc -o omp omp.c -fopenmp
```
Explanation:

`-o omp` → output executable named omp

`-fopenmp` → enables OpenMP directives

---

## Execution Instructions
Run the program with two arguments:
1. Input file for Task A (matrix file)
2. Input file for Task B (matrix file)

Example:
```bash
./omp src/A/A_T8_N400_CZ20.txt src/B/B_T8_N400_CZ20.txt
```

- Output will be saved automatically in `src/Output/` with names matching the input configuration.

---

## Input Files
- **Task A files**: `src/A/` – matrices for diagonal dominance and other operations
- **Task B files**: `src/B/` – matrices for the second task (maximum diagonal, B matrix generation, etc.)

File naming pattern:
```bash
A_T<Threads>_N<Size>_CZ<Chunk>.txt
```

Example: `A_T12_N1000_CZ100.txt` → 12 threads, 1000×1000 matrix, chunk size 100.

---

## Output Files
All results are saved to `src/Output/`:
- Contains task results: dominance check, maximum diagonal, B matrix, minimum element (via different methods)
- File names correspond to input configuration for easy correlation

---

## Running Experiments
- Modify thread count or chunk size in input files
- Compare sequential vs parallel execution
- Analyze speedup using the timing results in `docs/Times.xlsx`