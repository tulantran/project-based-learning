#include "bitwriter.h"

#include <stdio.h>
#include <stdlib.h>

struct BitWriter {
    FILE *underlying_stream; //file
    uint8_t byte; //buffer
    uint8_t bit_position; //serves pointer in buffer
};

BitWriter *bit_write_open(const char *filename) {

    BitWriter *bw = (BitWriter *) calloc(1, sizeof(BitWriter)); //allocates new BitWriter

    FILE *f = fopen(filename, "wb"); //creates buffer
    if (f == NULL) { //checks 
        free(bw);
        return NULL;
    } else { //initialize and return pointer
        bw->underlying_stream = f; 
        bw->byte = 0; //clearing fields
        bw->bit_position = 0;
        return bw;//pointer to newly allocated BitWriter struct
    }
}


void bit_write_close(BitWriter **pbuf) {//flushes and frees buffer
    if (*pbuf != NULL) {
        if ((*pbuf)->bit_position > 0) { //contains a bit that hasnt been written?
            fputc((*pbuf)->byte, (*pbuf)->underlying_stream); //write 
            fclose((*pbuf)->underlying_stream);
            free(*pbuf);
            *pbuf = NULL;
        }
    }
}


void bit_write_bit(BitWriter *buf, uint8_t bit) { //collects a byte and writes
    if (buf->bit_position > 7) {
        fputc(buf->byte, buf->underlying_stream);//writes
        buf->byte = 0;//clear fields
        buf->bit_position = 0;
    }
    buf->byte |= ((bit & 1) << buf->bit_position); //set bit
    buf->bit_position++;// increment pointer
}


//funcs call bit_write_bit corresponding amount of times

void bit_write_uint8(BitWriter *buf, uint8_t x) {
    for (int i = 0; i < 8; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

void bit_write_uint16(BitWriter *buf, uint16_t x) {
    for (int i = 0; i < 16; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}


void bit_write_uint32(BitWriter *buf, uint32_t x) {

    for (int i = 0; i < 32; i++) {
        bit_write_bit(buf, (x >> i) & 1);
    }
}

