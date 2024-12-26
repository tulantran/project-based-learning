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

- **Decompression (`dehuff`)**:
  - Reconstructs the Huffman tree from the binary format.
  - Decodes the compressed file back to its original content.

## Usage

### Compression
`huff -i <input_file> -o <output_file> huff -h`

- `-i <input_file>`: Specify the input file to compress.
- `-o <output_file>`: Specify the output file for the compressed data.
- `-h`: Display usage information.

### Decompression
dehuff -i <input_file> -o <output_file>   dehuff -h

- `-i <input_file>`: Specify the compressed file to decompress.
- `-o <output_file>`: Specify the output file for the decompressed data.
- `-h`: Display usage information.

## File Descriptions

### `huff.c`
- Implements the compression process.
- Key functions:
  - `fill_histogram`: Reads the input file and computes the frequency of each symbol.
  - `create_tree`: Constructs the Huffman tree based on the frequency histogram.
  - `fill_code_table`: Generates binary codes for each symbol using the Huffman tree.
  - `huff_compress_file`: Writes the compressed data to the output file.

### `dehuff.c`
- Implements the decompression process.
- Key functions:
  - `dehuff_decompress_file`: Reads the compressed file, reconstructs the Huffman tree, and writes the decompressed data to the output file.

## Dependencies

- Standard C libraries:
  - `stdio.h`
  - `stdlib.h`
  - `stdint.h`
  - `unistd.h`
  - `assert.h`
- Custom headers:
  - `bitreader.h`
  - `bitwriter.h`
  - `node.h`
  - `pq.h`

## Compilation

Compile the programs using a C compiler (e.g., GCC):
gcc -o huff huff.c bitreader.c bitwriter.c node.c pq.c gcc -o dehuff dehuff.c bitreader.c bitwriter.c node.c pq.c

## Notes

- The Huffman tree and prefix codes ensure the compressed data is decodable.
- The programs validate input/output files and handle errors gracefully.

