<p align="center">
  <img src="https://www.especial.gr/wp-content/uploads/2019/03/panepisthmio-dut-attikhs.png" alt="UNIWA" width="150"/>
</p>

<p align="center">
  <strong>UNIVERSITY OF WEST ATTICA</strong><br>
  SCHOOL OF ENGINEERING<br>
  DEPARTMENT OF COMPUTER ENGINEERING AND INFORMATICS
</p>

<p align="center">
  <a href="https://www.uniwa.gr" target="_blank">University of West Attica</a> ·
  <a href="https://ice.uniwa.gr" target="_blank">Department of Computer Engineering and Informatics</a>
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
  Co-supervisor: Michalis Iordanakis, Academic Scholar
</p>

<p align="center">
  <a href="https://ice.uniwa.gr/academic_sc_ho/" target="_blank">UNIWA Profile</a> ·
  <a href="https://scholar.google.com/citations?user=LiVuwVEAAAAJ&hl=en" target="_blank">Scholar</a>
</p>

</hr>

---

<p align="center">
  Athens, December 2024
</p>

---

<p align="center">
  <img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcS22iJggHfB24_pshOv0ibyq0JOnBPbLj4_tQ&s" width="250"/>
</p>

---

# README

## Parallel Computing using OpenMP

This repository provides an overview of a parallel computing project using **OpenMP**. The project focuses on **matrix manipulation**, **diagonal dominance verification**, and **parallel reduction techniques**.

---

## Table of Contents

| Section | Folder/File                                 | Description                                                      |
| ------: | ------------------------------------------- | ---------------------------------------------------------------- |
|       1 | `assign/`                                   | Assignment material for the OpenMP workshop                      |
|     1.1 | `assign/_Par_Sys_Ask_1_2024-25.pdf`         | Assignment description in English                                |
|     1.2 | `assign/_Παρ_Συσ_Ασκ_1_2024-25.pdf`         | Assignment description in Greek                                  |
|       2 | `docs/`                                     | Documentation and performance data for OpenMP parallel computing |
|     2.1 | `docs/Parallel-Computing-using-OpenMP.pdf`  | English documentation for OpenMP parallel computing              |
|     2.2 | `docs/Παράλληλος-Υπολογισμός-με-OpenMP.pdf` | Greek documentation for OpenMP parallel computing                |
|     2.3 | `docs/Times.xlsx`                           | Performance timing results                                       |
|       3 | `src/`                                      | Source code and input/output data for OpenMP exercises           |
|     3.1 | `src/omp.c`                                 | Main OpenMP program                                              |
|     3.2 | `src/A/`                                    | Input data files for exercise A                                  |
|   3.2.1 | `src/A/A_no_strict_diagonial.txt`           | Special test case for A                                          |
|   3.2.2 | `src/A/A_T1_N10_CZ2.txt`                    | Input for 1 thread, N=10, CZ=2                                   |
|   3.2.3 | `src/A/A_T1_N16_CZ3.txt`                    | Input for 1 thread, N=16, CZ=3                                   |
|   3.2.4 | `src/A/A_T1_N25_CZ5.txt`                    | Input for 1 thread, N=25, CZ=5                                   |
|   3.2.5 | `src/A/A_T1_N400_CZ20.txt`                  | Input for 1 thread, N=400, CZ=20                                 |
|   3.2.6 | `src/A/A_T1_N1000_CZ100.txt`                | Input for 1 thread, N=1000, CZ=100                               |
|   3.2.7 | `src/A/A_T12_N10_CZ2.txt`                   | Input for 12 threads, N=10, CZ=2                                 |
|   3.2.8 | `src/A/A_T12_N16_CZ3.txt`                   | Input for 12 threads, N=16, CZ=3                                 |
|   3.2.9 | `src/A/A_T12_N25_CZ5.txt`                   | Input for 12 threads, N=25, CZ=5                                 |
|  3.2.10 | `src/A/A_T12_N400_CZ20.txt`                 | Input for 12 threads, N=400, CZ=20                               |
|  3.2.11 | `src/A/A_T12_N1000_CZ100.txt`               | Input for 12 threads, N=1000, CZ=100                             |
|  3.2.12 | `src/A/A_T2_N10_CZ2.txt`                    | Input for 2 threads, N=10, CZ=2                                  |
|  3.2.13 | `src/A/A_T2_N16_CZ3.txt`                    | Input for 2 threads, N=16, CZ=3                                  |
|  3.2.14 | `src/A/A_T2_N25_CZ5.txt`                    | Input for 2 threads, N=25, CZ=5                                  |
|  3.2.15 | `src/A/A_T2_N400_CZ20.txt`                  | Input for 2 threads, N=400, CZ=20                                |
|  3.2.16 | `src/A/A_T2_N1000_CZ100.txt`                | Input for 2 threads, N=1000, CZ=100                              |
|  3.2.17 | `src/A/A_T4_N10_CZ2.txt`                    | Input for 4 threads, N=10, CZ=2                                  |
|  3.2.18 | `src/A/A_T4_N16_CZ4.txt`                    | Input for 4 threads, N=16, CZ=4                                  |
|  3.2.19 | `src/A/A_T4_N25_CZ5.txt`                    | Input for 4 threads, N=25, CZ=5                                  |
|  3.2.20 | `src/A/A_T4_N400_CZ20.txt`                  | Input for 4 threads, N=400, CZ=20                                |
|  3.2.21 | `src/A/A_T8_N10_CZ2.txt`                    | Input for 8 threads, N=10, CZ=2                                  |
|  3.2.22 | `src/A/A_T8_N16_CZ3.txt`                    | Input for 8 threads, N=16, CZ=3                                  |
|  3.2.23 | `src/A/A_T8_N25_CZ5.txt`                    | Input for 8 threads, N=25, CZ=5                                  |
|  3.2.24 | `src/A/A_T8_N400_CZ20.txt`                  | Input for 8 threads, N=400, CZ=20                                |
|     3.3 | `src/B/`                                    | Input data files for exercise B (same pattern as A)              |
|     3.4 | `src/Output/`                               | Output result files from OpenMP execution                        |
|       4 | `README.md`                                 | Project documentation                                            |
|       5 | `INSTALL.md`                                | Usage instructions                                               |

---

## 1. Project Overview

This project evaluates the performance and implementation of a multithreaded program using the **fork-join parallel execution model**. Key highlights:

- High-level **OpenMP techniques**
- Task division across threads
- Synchronized communication
- Performance scaling and speedup measurement between sequential and parallel execution

---

## 2. Problem Description

The software performs operations on square matrices \(N \times N\):

### 2.1 Task A: Diagonal Dominance Check

Check if matrix **A** is strictly diagonally dominant:

$$
|A_{ii}| > \sum_{j=0, j \neq i}^{N-1} |A_{ij}|
$$

### 2.2 Task B: Maximum Diagonal Element

Find the maximum absolute element \(m\) on the diagonal of **A**.

### 2.3 Task C: Generate Matrix B

Create matrix **B** based on \(m\):

$$
B_{ij} =
\begin{cases}
m - |A_{ij}| & i \neq j \\
m & i = j
\end{cases}
$$

### 2.4 Task D: Minimum Element in B

Compute the minimum absolute value in **B** using three methods:

1. **OpenMP reduction directive**
2. **Critical region protection**
3. **Binary Tree Algorithm for parallel reduction**

---

## 3. Implementation Details

### 3.1 Data Structures

- **A, B**: Dynamic 2D arrays storing matrix data
- **M**: Dynamic 1D array for the binary tree reduction
- **chunk**: Number of iterations per thread (parameter **CZ**)

### 3.2 Binary Tree Algorithm

Manually replaces the OpenMP reduction. Threads synchronize in phases, comparing pairs until a single minimum remains in the first position.

### 3.3 Parallel Directives

- `#pragma omp parallel` → Activates threads
- `#pragma omp for schedule(static, chunk)` → Distributes loop iterations
- `#pragma omp atomic` → Ensures atomic updates (e.g., dominance flag)
- `reduction(max: m)` → Handles maximum calculation automatically

---

## 4. Performance Analysis

- Execution times are measured to calculate **Speedup (S)**:

$$
S = \frac{T_{sequential}}{T_{parallel}}
$$

### 4.1 Test Parameters

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
