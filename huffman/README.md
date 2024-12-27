# Huffman Encoder and Decoder

This project implements a Huffman encoding and decoding system using C. It consists of two primary programs:

- `huff.c`: Compresses files using Huffman coding.
- `dehuff.c`: Decompresses files encoded with Huffman coding.

## Overview

Huffman coding is a lossless data compression algorithm. It uses variable-length binary codes for encoding symbols, where frequently occurring symbols have shorter codes, and infrequently occurring symbols have longer codes. This implementation uses custom data structures like priority queues and binary trees to perform Huffman encoding and decoding.

## Features

- **Compression (`huff`)**:
  - Reads the input file and constructs a frequency histogram.
  - Builds a Huffman tree and generates prefix codes for each symbol.
  - Compresses the input file into a binary format.
  - Validates input/output files and handles errors gracefully.

- **Decompression (`dehuff`)**:
  - Reconstructs the Huffman tree from the binary format.
  - Decodes the compressed file back to its original content.
  - Validates input/output files and handles errors gracefully.

## Usage

### Compilation
- To compile both programs, simply use the provided Makefile:  
`make`  
- This will generate `huff` and `dehuff` executables

To remove compiled binaries and intermediate files, run:  
`make clean`

### Compression
`huff -i <input_file> -o <output_file>` 

`huff -h`  

- `-i <input_file>`: Specify the input file to compress.
- `-o <output_file>`: Specify the output file for the compressed data.
- `-h`: Display usage information.

### Decompression
`dehuff -i <input_file> -o <output_file>`  

`dehuff -h`

- `-i <input_file>`: Specify the compressed file to decompress.
- `-o <output_file>`: Specify the output file for the decompressed data.
- `-h`: Display usage information.

## File Descriptions
`huff.c`: Implements the compression process using Huffman coding. Handles input/output files, constructs the Huffman tree, generates prefix codes, and writes the compressed data.    

`dehuff.c`: Implements the decompression process. Reconstructs the Huffman tree, validates the compressed data, and restores the original file.  
`bitreader.h` / `bitreader.c`: Provides utilities for reading binary data bit-by-bit from files.  
`bitwriter.h` / `bitwriter.c`: Provides utilities for writing binary data bit-by-bit to files.  
`node.h` / `node.c`: Defines the structure of a node in the Huffman tree and functions to manipulate nodes.  
`pq.h` / `pq.c`: Implements a priority queue, used for building the Huffman tree.  
`Makefile`: Automates the compilation process for the project, including huff and dehuff, and provides a make clean option for cleaning build artifacts.


