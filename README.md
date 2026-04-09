# Binary Image Encoder (Recursive Transliteration) 🖼️

A CLI-based digital image processing tool developed in **C** that implements an innovative binary image encoding strategy. This project was developed for the **Algorithms and Programming II** course at **Mackenzie University**.

## 🚀 The Challenge
Digital images are sets of pixels representing color, intensity, and position. For binary images (1-bit depth), each pixel is either black or white. This system transliterates an $N \times M$ image into a unique sequence of **P** (Black), **B** (White), and **X** (Mixed) characters based on recursive spatial decomposition.

## 🧠 Encoding Logic
The algorithm follows two fundamental rules:
1. **Uniform Images:** If an image is uniform, the code is simply the letter corresponding to the color: **B** for white or **P** for black.
2. **Mixed Images:** If the image has diverse colors, the code emits **X** and performs two cuts (horizontal and vertical) to produce up to four sub-images (quadrants).
   - The process recurses through each quadrant in order (1st, 2nd, 3rd, and 4th).
   - **Handling Indivisibility:** Since pixels are indivisible, left sub-images get an extra column and upper sub-images get an extra row when dimensions are odd.

## 🛠️ Technical Implementation
- **Recursion:** Implements a deep recursive function `decodificar_imagem` to traverse and decompose the image grid.
- **File I/O:** Specialized parser for **PBM (Portable Bitmap)** files in ASCII mode (**P1**), capable of handling "magic numbers", headers, and skipping comments marked with `#`.
- **CLI Interface:** Robust argument processing supporting manual input (`-m`) or file-based input (`-f`).
- **Memory Management:** Efficiently handles images up to the maximum resolution of **1024x768**.

## 💻 Usage Instructions
Compile the program:
```bash
gcc main.c -o ImageEncoder
```

### Options:
- **Help:** `./ImageEncoder -?` or `./ImageEncoder --help`.
- **Manual Input:** `./ImageEncoder -m` (prompts for width, height, and pixel values).
- **File Input:** `./ImageEncoder -f image.pbm` (reads and encodes a PBM file).

## 📊 Sample Output
An example of a 3x5 mixed image might generate a complex sequence such as: `XXPBPBXPBBPXBPP`.

## 👥 Contributors
- **José Maike Ferreira Ferro**
- Cauã Andrelino Maltez

---
*Developed as a practical exercise in Data Structures and Digital Image Foundations.*
