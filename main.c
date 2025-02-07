#include <stdlib.h>
#include <stdio.h>
#include "uarray.h"
#include "uarray2.h"
#include "solve_sudoku.h"
#include "unit_tests.h"

// int main()
// {
//     int w = 9;
//     int h = 9;
//     int marker = 100;

//     struct UArray2* matrix = UArray2_new(w, h, sizeof(int));

//     *((int *)UArray2_at(matrix, w - 1, h - 1)) = marker;

//     printf("marker: %d\n", *((int *)UArray2_at(matrix, w - 1, h - 1)));
//     printf("width: %d\n", UArray2_width(matrix));

//     UArray2_free(matrix);
// }



// int main(void)
// {
//     // Standard Sudoku board dimensions
//     int width = 9;
//     int height = 9;

//     // Create a new 9x9 matrix where each element is an int
//     struct UArray2 *sudokuBoard = UArray2_new(width, height, sizeof(int));

//     // Fill the board with numbers.
//     // Here, we simply assign a number between 1 and 9 to each cell.
//     // The value is calculated as: ((row * width + column) % 9) + 1.
//     for (int y = 0; y < UArray2_height(sudokuBoard); y++)
//     {
//         for (int x = 0; x < UArray2_width(sudokuBoard); x++)
//         {
//             int *cell = (int *)UArray2_at(sudokuBoard, x, y);
//             *cell = ((x + 3 * (y % 3) + categorize(y)) % 9) + 1;
//         }
//     }

//     // Print the Sudoku board to the console
//     for (int y = 0; y < UArray2_height(sudokuBoard); y++)
//     {
//         for (int x = 0; x < UArray2_width(sudokuBoard); x++)
//         {
//             int *cell = (int *)UArray2_at(sudokuBoard, x, y);
//             printf("%d ", *cell);
//         }
//         printf("\n");
//     }

//     printf("Is this a valid sudoku board? %d\n", isValidSudoku(sudokuBoard));

//     UArray2_free(sudokuBoard);

//     return 0;
// }

int main()
{
    test_invalid_sudoku_square();
}
