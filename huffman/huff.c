#include "bitreader.h"
#include "bitwriter.h"
#include "node.h"
#include "pq.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPT_ERR "huff:  unknown or poorly formatted option -%c\n"
#define USAGE                                                                                      \
    "Usage: huff -i infile -o outfile\n"                                                                                               \
    "       huff -h\n"

//structure for the prefix code for each letter 
typedef struct Code {
    uint64_t code;
    uint8_t code_length;
} Code;

uint32_t fill_histogram(FILE *fin, uint32_t *histo) {//reads each letter of file and increments frequency counter of letter and tracks file size
    uint32_t filesize = 0;
    int byte;

    while ((byte = fgetc(fin)) != EOF) {
        histo[byte]++;
        filesize++;
    }
    histo[0x00]++;//guarantees nodes for tree
    histo[0xFF]++;

    return filesize;
}

Node *create_tree(uint32_t *histo, uint16_t *num_leaves) { //creates huffman tree!
    *num_leaves = 0;
    PriorityQueue *pq = pq_create(); //initialize a priority queue (list)
    assert(pq != NULL);

    for (uint16_t symb = 0; symb < 256; symb++) {
        if (histo[symb] > 0) {
            Node *n = node_create((uint8_t) symb, histo[symb]); //create node for each letter that shows up in file
            enqueue(pq, n); //inserts into list in order of frequency
            *num_leaves += 1;
        }
    }

    pq_print(pq);

    while (!pq_size_is_1(pq)) { //build binary tree starting from least frequent letters (to give longest codes)
        Node *leftn = dequeue(pq);
        Node *rightn = dequeue(pq);
        Node *newnode = node_create(0, leftn->weight + rightn->weight); //make new generic node to serve as parent of 2 letters to re-enqueue
        newnode->left = leftn;
        newnode->right = rightn;

        enqueue(pq, newnode);
    }
    Node *root = dequeue(pq); // when there is only 1 node left (the root of the tree), free the PQ
    assert(pq_is_empty(pq));
    pq_free(&pq);
    return root;
}

void fill_code_table(Code *code_table, Node *node, uint64_t code, uint8_t code_length) { //Recursively fills a code table with binary codes for each symbol based on a Huffman tree.
    if (node->left != NULL) {
        fill_code_table(code_table, node->left, code, code_length + 1); //recursively traverse left subtree (no need to alter code since it will start as 0)
        code |= (uint64_t) 1 << code_length;//set appropriate code bit to 1 as we traverse right subtrees
        fill_code_table(code_table, node->right, code, code_length + 1);
    } else {
        code_table[node->symbol].code = code; // when traverse to a leaf, set code to symbol in table
        code_table[node->symbol].code_length = code_length; // record length
    }
}

void huff_write_tree(BitWriter *outbuf, Node *node) { // writes decodable notation of tree
    if (node->left == NULL) {
        bit_write_bit(outbuf, 1);
        bit_write_uint8(outbuf, node->symbol);
    } else {
        huff_write_tree(outbuf, node->left);
        huff_write_tree(outbuf, node->right);
        bit_write_bit(outbuf, 0);
    }
}

void huff_compress_file(BitWriter *outbuf, FILE *fin, uint32_t filesize, uint16_t num_leaves,// writes compressed file!
    Node *code_tree, Code *code_table) {
    bit_write_uint8(outbuf, 'H'); //indicating data
    bit_write_uint8(outbuf, 'C');
    bit_write_uint32(outbuf, filesize);//data
    bit_write_uint16(outbuf, num_leaves);
    huff_write_tree(outbuf, code_tree);

    while (true) { 
        int b = fgetc(fin);//reads in symbol
        if (b == EOF) {
            break;
        }
        uint64_t code = code_table[b].code; //retrieves code
        uint8_t code_length = code_table[b].code_length;
        for (uint8_t i = 0; i < code_length; ++i) {
            bit_write_bit(outbuf, code & 1);//writes to compressed file
            code >>= 1;
        }
    }
}




int main(int argc, char **argv) {

    // HANDLE OPTIONS AND FILE IO
    bool iused = false;
    bool oused = false;
    FILE *infile = stdin;
    int opt;
    char *outfile;

    while ((opt = getopt(argc, argv, "i:o:h")) != -1) {
        switch (opt) {
        case 'i':
            iused = true;
            infile = fopen(optarg, "rb");
            if (infile == NULL) {
                fprintf(stderr, OPT_ERR USAGE, opt);
                exit(1);
            }
            break;

        case 'o':
            oused = true;
            outfile = optarg;
            if (outfile == NULL) {
                fprintf(stderr, OPT_ERR USAGE, opt);
                exit(1);
            }
            break;
        case 'h': printf(USAGE); exit(0);
        default:
            fprintf(stderr, OPT_ERR USAGE, optopt);
            exit(1);
            break;
        }
    }

    if (!iused) {
        fprintf(stderr, "huff:  -i option is required\n" USAGE);
        exit(1);
    }
    if (!oused) {
        fprintf(stderr, "huff:  -o option is required\n" USAGE);
        exit(1);
    }

    //HISTOGRAM SET UP
    uint32_t histo[256] = { 0 };
    uint32_t filesize = fill_histogram(infile, histo);

    //MAKE TREE
    uint16_t num_leaves = 0;
    Node *root = create_tree(histo, &num_leaves);

    //CODE TABLE
    Code codestru[256];
    for (int i = 0; i < 256; i++) {
        codestru[i].code = 0;
        codestru[i].code_length = 0;
    }
    fill_code_table(codestru, root, 0, 0);
    fseek(infile, 0, SEEK_SET);

    //WRITE 
    BitWriter *outbuf = bit_write_open(outfile);
    assert(outbuf != NULL);
    huff_compress_file(outbuf, infile, filesize, num_leaves, root, codestru);

    //TIE LOOSE ENDS
    bit_write_close(&outbuf);
    fclose(infile);
    node_free(&root);
}

