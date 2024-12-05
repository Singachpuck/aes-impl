#ifndef UTILS_H
#define UTILS_H

#define BLOCK_DIM_SIZE 4
#define BLOCK_SIZE 16
#define KEY_SIZE 128
#define N_ROUNDS 10

#include <stdint.h>

typedef struct Block {
    uint8_t data[BLOCK_DIM_SIZE][BLOCK_DIM_SIZE];
} Block;
#endif //UTILS_H
