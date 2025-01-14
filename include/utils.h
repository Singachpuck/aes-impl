#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>

#include "constants.h"

#define BLOCK_DIM_SIZE 4
#define BLOCK_SIZE 16
#define KEY_SIZE 128
#define N_ROUNDS 10

typedef struct Block {
    uint8_t data[BLOCK_DIM_SIZE][BLOCK_DIM_SIZE];
} Block;

void print_hex(const uint8_t* bytes, const size_t size);
void set_cek(const uint8_t* cek);
const uint8_t* get_cek();
const uint8_t* get_round_key(uint32_t round);
void key_expansion();
void add_round_key(const uint32_t round, Block* block);
void sub_bytes(Block* block);
void shift_rows(Block* block);
void mix_columns(Block* block);

#endif //UTILS_H
