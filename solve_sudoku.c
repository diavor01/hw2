#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
// #include "uarray2.h"

#include "solve_sudoku.h"

int return_square(int x, int y)
{
    return ( (y / 3 ) * 3 ) + (x / 3 ) + 1;
}


int check_uniqueness(struct indices value)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = i + 1; j < 9; j++)
        {
            if (value.row[j] == value.row[i] || value.col[j] == value.col[i] || 
                value.square[j] == value.square[i]) {
                return 0;
            }
        }
    }

    return 1;
}


int isValidSudoku(struct UArray2* matrix) 
{
    struct indices* values = malloc(10 * sizeof(struct indices));
    int size[10] = {0};

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            int value = *(int*)UArray2_at(matrix, j-1, i-1);

            assert(value > 0 && value < 10);
            int s = size[value];
            values[value].row[s] = i;
            values[value].col[s] = j;
            values[value].square[s] = return_square(j-1, i-1);
            size[value]++;

        }
    }
    for (int i = 1; i < 10; i++)
    {
        printf("Value %d\n", i);
        for (int j = 0; j < 9; j++)
        {
            printf("(%d %d %d)\n", values[i].row[j], values[i].col[j],  values[i].square[j]);
        }

        if (check_uniqueness(values[i]) == 0)
        {
            printf("Failed uniqueness test at i = %d\n", i);
            return 0;
        }
    }

    free(values);

    return 1;
}


// int main()
// {
    
// }