#include <stdint.h>

int32_t M __attribute__((aligned(32))) = 5;
int32_t N __attribute__((aligned(32))) = 5;
int32_t F __attribute__((aligned(32))) = 3;

// Input 7x7 (senza padding, lineare in row-major)
int32_t i[49] = {
     1,  2,  3,  4,  5,  6,  7,
     8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49
};

// Filtro 3x3
int32_t f[9] = {
     1,  0, -1,
     1,  0, -1,
     1,  0, -1
};

// Output atteso (5x5)
int32_t golden_o[25] = {
    -6, -6, -6, -6, -6,
    -6, -6, -6, -6, -6,
    -6, -6, -6, -6, -6,
    -6, -6, -6, -6, -6,
    -6, -6, -6, -6, -6
};

// Buffer per l’output del kernel
int32_t o[25] = {0};
