#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

// void print_hex(const uint8_t* bytes, const size_t size) {
//   for (int j = 0; j < size; j++) {
//     printf("%02x ", bytes[j]);
//     // printf("%c ", block->data[j][i]);
//   }
//   printf("\n");
// }

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

// TODO: Create Round key generation
int main(void) {
  uint32_t block_i = 0;
  Block block = {0};

  const uint8_t* plaintext = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
  // const uint8_t* plaintext = "HELLO";
  // const uint8_t ciphertext[strlen(plaintext)];
  const uint8_t* CEK = "dZ8&hQk3@X$NpV1M";

  set_cek(CEK);
  // print_hex(get_cek(), CEK_BYTE_N);

  key_expansion();

  while (assign_next_block(&block, &block_i, plaintext, strlen(plaintext)) != NULL) {
    uint32_t round = 0;
    add_round_key(round, &block);
    round++;
    for (; round < N_ROUNDS; round++) {
      sub_bytes(&block);
      shift_rows(&block);
      mix_columns(&block);
      add_round_key(round, &block);
    }

    sub_bytes(&block);
    shift_rows(&block);
    add_round_key(round, &block);

    print_hex(block.data, BLOCK_SIZE);
  }

  // for (int i = 1; i <= ROUNDS_N; i++) {
  //   print_hex(get_round_key(i), CEK_BYTE_N);
  // }

  printf("AES Works!\n");
  return EXIT_SUCCESS;
}