#include "bitreader.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct BitReader {
    FILE *underlying_stream;//file pointer
    uint8_t byte;//buffer
    uint8_t bit_position;//points to bit in buffer
};

BitReader *bit_read_open(const char *filename) { //Open binary filename using fopen() and return a pointer to a BitReader. On error, return NULL. 
  
    BitReader *br = (BitReader *) calloc(1, sizeof(BitReader)); //allocate a new BitReader
    if (br == NULL) {
        free(br);
        return NULL;
    }

    FILE *f = fopen(filename, "r"); //open the filename for reading as a binary file, storing the result in FILE *f 
    if (f == NULL) {
        free(br);
        return NULL;
    } else {
        br->byte = 0; //clear the byte field of the BitReader to 0 
        br->bit_position = 8; //clear the byte field of the BitReader to 0 
        br->underlying_stream = f; //store f in the BitReader field underlying_stream 
        return br;// return a pointer to the new BitReader
    }
}


void bit_read_close(BitReader **pbuf) { //Using values in the BitReader pointed to by *pbuf, close (*pbuf)->underlying_stream, free the BitReader object, and set the *pbuf pointer to NULL.
    if (*pbuf != NULL) {
        assert(fclose((*pbuf)->underlying_stream) != EOF);
        free(*pbuf);
        *pbuf = NULL;
    } else {
        exit(1);
    }
}

uint8_t bit_read_bit(BitReader *buf) { //main reading function. It reads a single bit using values in the BitReader pointed to by buf.

    if (buf->bit_position > 7) {
        int byte = fgetc(buf->underlying_stream);//read byte 
        assert(byte != EOF);

        buf->bit_position = 0;
        buf->byte = (uint8_t) byte;
    }

    uint8_t bit = (buf->byte >> buf->bit_position) & 1;// Simplified bit extraction using bitwise AND
    buf->bit_position++; //next bit
    return bit;
}


uint8_t bit_read_uint8(BitReader *buf) { //Read 8 bits from buf by calling bit_read_bit() 8 times. Collect these bits into a uint8_t starting with the LSB.

    uint8_t byte = 0x00;
    for (int i = 0; i < 8; i++) {
        uint8_t b = bit_read_bit(buf);
        byte |= b << i;
    }

    return byte;
}
uint16_t bit_read_uint16(BitReader *buf) {//Read 16 bits from buf by calling bit_read_bit() 16 times. Collect these bits into a uint16_t starting with the LSB
 
    uint16_t word = 0x0000;
    for (int i = 0; i < 16; i++) {
        uint8_t b = bit_read_bit(buf);
        word |= (uint16_t) b << i;
    }

    return word;
}

uint32_t bit_read_uint32(BitReader *buf) {//Read 32 bits from buf by calling bit_read_bit() 32 times. Collect these bits into a uint32_t starting with the LSB

    uint32_t word = 0x00000000;
    for (int i = 0; i < 32; i++) {
        uint8_t b = bit_read_bit(buf);
        word |= (uint32_t) b << i;
    }

    return word;
}
