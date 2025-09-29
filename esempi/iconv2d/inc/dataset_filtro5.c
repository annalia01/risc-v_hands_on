#include <stdint.h>
#include "runtime.h"

int32_t M __attribute__((aligned(32))) = 3;
int32_t N __attribute__((aligned(32))) = 3;
int32_t F __attribute__((aligned(32))) = 5;

// Input: matrice 7x7 riempita con 1..49 (in row-major)
int32_t i[(3+5-1)*(3+5-1)] __attribute__((aligned(32))) = {
     1,  2,  3,  4,  5,  6,  7,
     8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28,
    29, 30, 31, 32, 33, 34, 35,
    36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49
};

// Filtro 5x5: pattern [+1, 0, -1, 0, +1] ripetuto
int32_t f[25] __attribute__((aligned(32))) = {
     1,  0, -1,  0,  1,
     1,  0, -1,  0,  1,
     1,  0, -1,  0,  1,
     1,  0, -1,  0,  1,
     1,  0, -1,  0,  1
};

// Output: 3x3, inizializzato a 0
int32_t o[9] __attribute__((aligned(32))) = {0};

// Golden output calcolato a mano (3x3)
int32_t golden_o[9] __attribute__((aligned(32))) = {
    85,  90,  95,
   120, 125, 130,
   155, 160, 165
};
