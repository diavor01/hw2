#include <stdlib.h>
#include <stdio.h>

struct indices
{
    int row[10];
    int col[10];
};

bool check_uniqueness(int s, struct indices value)
{
    for (int i = 0; i < s - 1; i++)
    {
        for (int j = i + 1; j < s; j++)
        {
            if (value.row[j] == value.row[i])
            {
                printf("for %d and %d, %d = %d\n", j, i, value.row[j], value.row[i]);
                return false;
            }
            if (value.col[j] == value.col[i])
            {
                printf("%d = %d\n", j, i, value.col[j], value.col[i]);
                return false;                
            }
        }
    }

    return true;
}


bool isValidSudoku(char** board, int boardSize, int* boardColSize) 
{
    struct indices* values = malloc(10 * sizeof(struct indices));
    int size[10] = {0};

    for (int i = 1; i < 10; i++)
    {
        for (int j = 1; j < 10; j++)
        {
            int value = board[i-1][j-1] - '0';

            if (board[i-1][j-1] != '.')
            {
                assert(value > 0 && value < 10);
                int s = size[value];
                values[value].row[s] = i;
                values[value].col[s] = j;
                size[value]++;
            }
        }
    }
    for (int i = 1; i < 10; i++)
    {
        int s = size[i];
        printf("Value %d\n", i);
        for (int j = 0; j < s; j++)
        {
            printf("(%d %d)\n", values[i].row[j], values[i].col[j]);
        }

        if (!check_uniqueness(s, values[i]))
        {
            printf("Checked uniqueness\n");
            return false;
        }
    }


    free(values);

    return true;
}