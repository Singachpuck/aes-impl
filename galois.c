#include "galois.h"


uint8_t gmul2(uint8_t a) {
    uint8_t h = a & 0x80; //high bit
    uint8_t b = a << 1;
    if (h == 0x80) b = b ^ 0x1b;
    return b;
}

uint8_t gmul3(uint8_t a) {
    return a ^ gmul2(a);
}