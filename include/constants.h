#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifndef CEK_BIT_LEN
#define CEK_BIT_LEN 128
#endif

#if CEK_BIT_LEN == 128
#define CEK_BYTE_N 16
#define CEK_WORD_N 4
#define ROUNDS_N 10
#endif

#endif //CONSTANTS_H
