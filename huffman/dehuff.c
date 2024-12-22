#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPT_ERR "dehuff:  unknown or poorly formatted option -%c\n"
#define USAGE                                                                                      \
    "Usage: dehuff -i infile -o outfile\n"                                                         \
    "       dehuff -v -i infile -o outfile\n"                                                      \
    "       dehuff -h\n"

Node *stack[100]; // Stack for constructing the Huffman tree
int stackptr = 0;

void stack_push(Node *node) { // add to top of stack 
    assert(!(stackptr >= 100));
    stack[stackptr] = node;
    stackptr++;
}

Node *stack_pop(void) { // remove from top of stack
    assert(stackptr > 0);
    int remove = stackptr - 1;
    stackptr--;
    return stack[remove];
}
void dehuff_decompress_file(FILE *fout, BitReader *inbuf) {

    uint8_t type1 = bit_read_uint8(inbuf);// read in identifiers and ensure they are 'HC'
    uint8_t type2 = bit_read_uint8(inbuf);
    assert(type1 == 'H');
    assert(type2 == 'C');

    uint32_t filesize = bit_read_uint32(inbuf); // read in filesize and num_leaves
    uint16_t num_leaves = bit_read_uint16(inbuf);
 
    uint32_t num_nodes = 2 * num_leaves - 1; //calculate total leaves

    Node *node;
    uint8_t rbit;

    // rebuild the Huffman tree using an iterative process and a stack
    for (uint32_t i = 0; i < num_nodes; i++) {
        rbit = bit_read_bit(inbuf); // determine the node type (leaf or internal)
        if (rbit == 1) {
            // leaf node: read the symbol it represents
            uint8_t symb = bit_read_uint8(inbuf);
            node = node_create(symb, 0); // create a leaf node with the given symbol
        } else {
            // internal node: construct a parent node for the two most recent nodes
            node = node_create(0, 0);
            node->right = stack_pop(); // the most recently added node becomes the right child
            node->left = stack_pop();  // the next node becomes the left child
        }
        stack_push(node); // push the newly created node back onto the stack
    }

    // the final node on the stack represents the root of the Huffman tree
    Node *code_tree = stack_pop();

    // decode the compressed file by traversing the reconstructed Huffman tree
    for (uint32_t i = 0; i < filesize; i++) {
        node = code_tree;

        // navigate the tree based on the bit stream until a leaf node is reached
        while (1) {
            rbit = bit_read_bit(inbuf); // read the next bit to determine direction
            node = (rbit == 0) ? node->left : node->right;

            if (node->left == NULL) {
                break; // stop traversal when a leaf node is reached
            }
        }
        fputc(node->symbol, fout); // write the decoded symbol to the output file
    }
    node_free(&code_tree); // release memory allocated for the Huffman tree
}

int main(int argc, char **argv) {
    // command-line argument flags to track input and output options
    bool iused = false; // tracks whether an input file is specified
    bool oused = false; // tracks whether an output file is specified
    char *infile;       // stores the name of the input file
    int opt;
    FILE *outfile;      // file pointer for the output file

    // parse and validate command-line options
    while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
        switch (opt) {
        case 'i':
            iused = true;
            infile = optarg; // capture input file name
            if (infile == NULL) {
                fprintf(stderr, OPT_ERR USAGE, opt);
                exit(1);
            }
            break;

        case 'o':
            oused = true;
            outfile = fopen(optarg, "wb"); // open output file in binary write mode
            if (outfile == NULL) {
                fprintf(stderr, OPT_ERR USAGE, opt);
                exit(1);
            }
            break;

        case 'h':
            printf(USAGE); // display usage information
            exit(0);

        default:
            fprintf(stderr, OPT_ERR USAGE, optopt); // handle invalid options
            exit(1);
            break;
        }
    }

    // ensure required options are specified
    if (!iused) {
        fprintf(stderr, "dehuff:  -i option is required\n" USAGE);
        exit(1);
    }
    if (!oused) {
        fprintf(stderr, "dehuff:  -o option is required\n" USAGE);
        exit(1);
    }

    // perform decompression
    BitReader *read = bit_read_open(infile); // initialize bit reader for the input file
    dehuff_decompress_file(outfile, read);  // decode the compressed file
    bit_read_close(&read);                  // close the bit reader
    fclose(outfile);                        // close the output file
}
