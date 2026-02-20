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

# INSTALL

## Parallel Computing using OpenMP

This repository contains laboratory simulations and source code for **Parallel Computing using OpenMP**, developed as part of the **Parallel Systems course** at the **University of West Attica**. The focus is on **matrix operations, diagonal dominance checking, and parallel reduction algorithms** using the **fork-join model**.

All exercises are implemented in **C** using **OpenMP**.

---

## 1. Prerequisites

### 1.1 Required Software

- **GCC Compiler** (version supporting OpenMP, typically ≥ 4.2)
- **OpenMP library support**
- Operating System: Linux, macOS, or Windows (via WSL or MinGW)

### 1.2 Optional Software

- **Text Editor / IDE** for source code editing (VSCode, CLion, etc.)
- **Spreadsheet Viewer** for analyzing timing results (`docs/Times.xlsx`)

---

## 2.Installation Steps

### 2.1 Clone the Repository

Clone using Git:

```bash
git clone https://github.com/Parallel-Systems-aka-Uniwa/OpenMP.git
```

Alternatively, download the ZIP file and extract it locally.

### 2.2 Navigate to Project Directory

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

## 3. Compilation Instructions

Compile the OpenMP C program using GCC:

```bash
gcc -o omp omp.c -fopenmp
```

Explanation:

`-o omp` → output executable named omp

`-fopenmp` → enables OpenMP directives

---

## 4. Execution Instructions

Run the program with two arguments:

1. Input file for Task A (matrix file)
2. Input file for Task B (matrix file)

Example:

```bash
./omp src/A/A_T8_N400_CZ20.txt src/B/B_T8_N400_CZ20.txt
```

- Output will be saved automatically in `src/Output/` with names matching the input configuration.

---

## 5. Input Files

- **Task A files**: `src/A/` – matrices for diagonal dominance and other operations
- **Task B files**: `src/B/` – matrices for the second task (maximum diagonal, B matrix generation, etc.)

File naming pattern:

```bash
A_T<Threads>_N<Size>_CZ<Chunk>.txt
```

Example: `A_T12_N1000_CZ100.txt` → 12 threads, 1000×1000 matrix, chunk size 100.

---

## 6. Output Files

All results are saved to `src/Output/`:

- Contains task results: dominance check, maximum diagonal, B matrix, minimum element (via different methods)
- File names correspond to input configuration for easy correlation

---

## 7. Running Experiments

- Modify thread count or chunk size in input files
- Compare sequential vs parallel execution
- Analyze speedup using the timing results in `docs/Times.xlsx`
