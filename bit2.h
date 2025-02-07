#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

#include "bit.h"
typedef struct Bit_T *bit;

typedef struct Bit2_T {
    int rows;
    int cols;
    Bit_T bit_arr;
} Bit2_T;

Bit2_T Bit2_new(int rows, int cols);
int Bit2_put(Bit2_T *bit2, int row, int col, int bit);
int Bit2_get(Bit2_T bit2, int row, int col);
void Bit2_free(Bit2_T *bit2);
int Bit2_width(Bit2_T bit2);
int Bit2_height(Bit2_T bit2);
void Bit2_map_row_major(Bit2_T bit2, void apply(
    int row, int col, Bit2_T bit2, int value, void *cl), void *cl);
void Bit2_map_col_major(Bit2_T bit2, void apply(
    int row, int col, Bit2_T bit2, int value, void *cl), void *cl);

#endif