# Binary Image Encoder (Recursive Transliteration) 🖼️

A CLI-based digital image processing tool developed in **C** that implements an innovative binary image encoding strategy. This project was developed for the **Algorithms and Programming II** course at **Mackenzie University**.

## 🚀 The Challenge
Digital images are sets of pixels representing color and intensity. For binary images (1-bit depth), each pixel is either black or white. This system transliterates an $N \times M$ image into a unique sequence of **P** (Black), **B** (White), and **X** (Mixed) characters based on recursive spatial decomposition.

## 🧠 Encoding Logic
The algorithm follows two fundamental rules:
1. **Uniform Images:** If all pixels are the same color, the code is simply `B` or `P`.
2. **Mixed Images:** If colors differ, the code emits `X`, and the image is split into four quadrants (sub-images). 
   - The process recurses through each quadrant in order (1st to 4th).
   - **Handling Indivisibility:** Since pixels are indivisible units, left/upper sub-images receive an extra row/column when dimensions are odd.

## 🛠️ Technical Implementation
- **Recursion:** Implements a deep recursive function `decodificar_imagem` to traverse the image grid.
- **File I/O:** Specialized parser for **PBM (Portable Bitmap)** files in ASCII mode (P1), capable of handling headers and skipping comments.
- **CLI Interface:** Robust argument processing supporting manual input (`-m`) or file-based input (`-f`).
- **Memory Management:** Handles images up to $1024 \times 768$ resolution.

## 💻 Usage Instructions
Compile the program:
  ```bash
  gcc main.c -o ImageEncoder

