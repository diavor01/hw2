#ifndef SUDOKU_INCLUDED
#define SUDOKU_INCLUDED

#include "uarray2.h"

struct indices
{
    int row[10];
    int col[10];
    int square[10];
};
int return_square(int x, int y);
int check_uniqueness(struct indices value);
int isValidSudoku(struct UArray2* matrix);

#endif