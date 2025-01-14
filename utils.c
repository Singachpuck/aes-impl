#include "utils.h"

#include <stdio.h>
#include <string.h>

#include "galois.h"

uint8_t K[CEK_BYTE_N];
uint32_t W[4*(ROUNDS_N + 1)];

uint8_t sbox_lookup[] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

void print_hex(const uint8_t* bytes, const size_t size) {
  for (int j = 0; j < size; j++) {
    printf("%02x", bytes[j]);
    // printf("%c ", block->data[j][i]);
  }
  printf("\n");
}

void print_words_hex(const uint32_t* words, const size_t size) {
  for (int j = 0; j < size; j++) {
    printf("%08x ", words[j]);
    // printf("%c ", block->data[j][i]);
  }
  printf("\n");
}

void set_cek(const uint8_t* cek) {
    memcpy(K, cek, CEK_BYTE_N);
}

const uint8_t* get_cek() {
    return K;
}

uint32_t* get_cek_words() {
    return (uint32_t*) K;
}

const uint8_t* get_round_key(const uint32_t round) {
    if (round < 0 || round > ROUNDS_N) {
        printf("Rounds must be withing range from %i to %i\n", 0, ROUNDS_N);
        return NULL;
    }
    return (uint8_t*) &W[round * 4];
}

// TODO: Write unit test
uint32_t rotate_word_left(uint32_t word, uint32_t n) {
    // Left shift by 8 bits and right shift by 24 bits, then combine with OR
    n %= 4;
    return (word << (n * 8)) | (word >> (32 - (n * 8)));
}

/**
* Exponentiates to a value in Galois field
*/
uint8_t rcon(uint32_t value) {
    uint8_t c = 1;

    if (value == 0) return 0;
    while (value != 1) {
        c = gmul2(c);
        value--;
    }
    return c;
}

// TODO: unit test
void sbox(uint8_t* a) {
    *a = sbox_lookup[*a];
}

// TODO: unit test
uint32_t subWord(uint32_t a) {
    uint8_t* a_bytes = (uint8_t*) &a;
    for (int i = 0; i < 4; i++) {
        sbox(&a_bytes[i]);
    }
    return *a_bytes;
}

/*
  for (i from 0 to 4R - 1) {
      //The first words (W[0] thru W[N-1]) are equal to the words of the original key.
      //For a 128-bit key, this means that the first round key is equal to the original key.
      if (0 < i < N) W[i] = K[i];

      //Perform operations on the last word of each N-length cycle before XOR-ing.
      else if (i >= N and i == 0 mod N)
        W[i] = W[i-N] ⊕ subWord(rotate(W[i-1])) ⊕ rcon(i/N);

      //For a 256-bit key length only.
      else if (i >= N and N == 8 and i == 4 mod N)
        W[i] = W[i-N] ⊕ subWord(W[i-1]);

      //Typical case
      else W[i] = W[i-N] ⊕ W[i-1];
}
*/

// TODO: unit test
void key_expansion() {
	uint32_t* cek_words = get_cek_words();

    print_hex((uint8_t*) cek_words, CEK_BYTE_N);
    for (int i = 0; i < 4*(ROUNDS_N + 1); i++) {
		if (i < CEK_WORD_N)
			W[i] = cek_words[i];
		else if (i % CEK_WORD_N == 0)
            W[i] = W[i-CEK_WORD_N] ^ subWord(rotate_word_left(W[i-1], 1)) ^ rcon(i/CEK_WORD_N);
		else if (CEK_WORD_N > 6 && i % CEK_WORD_N == 4)
            W[i] = W[i-CEK_WORD_N] ^ subWord(W[i-1]);
		else
            W[i] = W[i-CEK_WORD_N] ^ W[i-1];
    }

    print_words_hex(W, 4*ROUNDS_N);
    print_hex(W, 4*ROUNDS_N*4);
}

// TODO: unit test
void add_round_key(const uint32_t round, Block* block) {
    const uint8_t* key = get_round_key(round);
    for (int i = 0; i < CEK_BYTE_N; i++) {
        *(((uint8_t*) block->data) + i) ^= key[i];
    }
}

// TODO: unit test
void sub_bytes(Block* block) {
    for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
        for (int j = 0; j < BLOCK_DIM_SIZE; j++) {
            sbox(&block->data[i][j]);
        }
    }
}

// TODO: unit test
void shift_rows(Block* block) {
	for (int i = 1; i < BLOCK_DIM_SIZE; i++) {
        uint32_t* current = (uint32_t*) block->data[i];
		*current = rotate_word_left(*current, i);
    }
}

// TODO: unit test
void mix_columns(Block* block) {
	for (int i = 0; i < BLOCK_DIM_SIZE; i++) {
        uint8_t b1 = block->data[0][i];
        uint8_t b2 = block->data[1][i];
        uint8_t b3 = block->data[2][i];
        uint8_t b4 = block->data[3][i];

        block->data[0][i] = gmul2(b1) ^ gmul3(b2) ^ b3 ^ b4;
        block->data[1][i] = b1 ^ gmul2(b2) ^ gmul3(b3) ^ b4;
        block->data[2][i] = b1 ^ b2 ^ gmul2(b3) ^ gmul3(b4);
        block->data[3][i] = gmul3(b1) ^ b2 ^ b3 ^ gmul2(b4);
    }
}