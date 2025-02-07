#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "bit2.h"




Bit2_T Bit2_new(int rows, int cols) 
{
    Bit2_T bit2;
    bit2.bit_arr = Bit_new(rows * cols);
    assert( bit2.bit_arr != NULL);
    bit2.rows = rows;
    bit2.cols = cols;

    return bit2;
}

int Bit2_put(Bit2_T *bit2, int row, int col, int bit)
{
    assert(row >= 0 && row < bit2->rows);
    assert(col >= 0 && col < bit2->cols);
    int index = row * bit2->cols + col;
    int last_bit = Bit_put(bit2->bit_arr, index, bit);
    
    return last_bit;
}

int Bit2_get(Bit2_T bit2, int row, int col)
{
    assert(row >= 0 && row < bit2.rows);
    assert(col >= 0 && col < bit2.cols);
    int index = row * bit2.cols + col;
    int curr_bit = Bit_get(bit2.bit_arr, index);
    
    return curr_bit;
}

void Bit2_free(Bit2_T *bit2)
{
    assert(bit2 != NULL);
    Bit_free(&bit2->bit_arr);
    bit2->bit_arr = NULL;

    free(bit2);
    bit2 = NULL;
}

int Bit2_width(Bit2_T bit2)
{
    return bit2.cols;
}

int Bit2_height(Bit2_T bit2)
{
    return bit2.rows;
}

void Bit2_map_row_major(Bit2_T bit2, void apply(
    int row, int col, Bit2_T bit2, int value, void *cl), void *cl) 
{
	for (int r = 0; r < bit2.rows; r++) 
    {
        for (int c = 0; c < bit2.cols; c++) 
        {
            int value = Bit2_get(bit2, r, c);
            apply(r, c, bit2, value, cl);
        }
    }
}

void Bit2_map_col_major(Bit2_T bit2, void apply(
    int row, int col, Bit2_T bit2, int value, void *cl), void *cl) 
{
	for (int c = 0; c < bit2.cols; c++) 
    {
        for (int r = 0; r < bit2.rows; r++) 
        {
            int value = Bit2_get(bit2, r, c);
            apply(r, c, bit2, value, cl);
        }
    }
} 