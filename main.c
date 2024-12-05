#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void print_block(const Block *block) {
  for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
    for (int j = 0; j < BLOCK_DIM_SIZE; j++) {
      printf("%02x ", block->data[j][i]);
      // printf("%c ", block->data[j][i]);
    }
    printf("\n");
  }
  printf("\n");
}

uint8_t* add_PKCS_7_padding(const Block* block, const uint8_t padding_size) {
  if (!padding_size) {
    return NULL;
  }
  const uint8_t padding_byte = padding_size;
  uint8_t* padd_ref = (uint8_t*) block->data + (BLOCK_SIZE - padding_size);
  memset(padd_ref, padding_byte, padding_size);
  return padd_ref;
}

uint8_t* assign_next_block(Block* block, uint32_t* block_i, const uint8_t* plaintext, const uint64_t text_size) {
  const uint64_t current_pos = *block_i * BLOCK_SIZE;
  int8_t to_copy = BLOCK_SIZE;
  if (current_pos + BLOCK_SIZE >= text_size) {
    to_copy = text_size - current_pos;

    if (to_copy <= 0) {
      return NULL;
    }

    const uint8_t padding_size = BLOCK_SIZE - to_copy;
    add_PKCS_7_padding(block, padding_size);
  }
  memcpy(block->data, plaintext + current_pos, to_copy);

  (*block_i)++;
  return (uint8_t*) block->data;
}

int main(void) {
  uint32_t block_i = 0;
  Block block = {0};

  const uint8_t* plaintext = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  // const uint8_t* plaintext = "HELLO";

  while (assign_next_block(&block, &block_i, plaintext, strlen(plaintext)) != NULL) {
    //first round
    // addRoundKey(roundKeys[0], block);    //intermediate rounds
    // for (int i = 0; i < N_ROUNDS - 1; i++) {
    //   subBytes(block);
    //   shiftRows(block);
    //   mixColumns(block);
    //   addRoundKey(roundKeys[i], block);
    // }
    //
    // //last round
    // subBytes(block);
    // shiftRows(block);
    // addRoundKey(roundKeys[numRounds - 1], block);
    print_block(&block);
  }

  printf("AES Works!\n");
  return EXIT_SUCCESS;
}