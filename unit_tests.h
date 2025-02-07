#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "solve_sudoku.h"

void test_return_square()
{
    int x = 4;
    int y = 0;
    assert(2 == return_square(x, y));

    x = 0;
    y = 7;
    assert(7 == return_square(x, y));

    x = 8;
    y = 5;
    assert(6 == return_square(x, y));

    printf("Tests passed sucessfully\n");
}


// Test 1: Duplicate rows.
void test_duplicate_rows(void) {
    struct indices idx = {
        .row = {1, 1, 2, 3, 4, 5, 6, 7, 8},
        .col = {0, 1, 2, 3, 4, 5, 6, 7, 8},
        .square = {0, 1, 2, 3, 4, 5, 6, 7, 8}
    };
    // Expect duplicate in row: function should return 0.
    assert(check_uniqueness(idx) == 0);
    printf("test_duplicate_rows passed.\n");
}

// Test 2: Duplicate columns.
void test_duplicate_cols(void) {
    struct indices idx = {
        .row = {0, 1, 2, 3, 4, 5, 6, 7, 8},
        .col = {1, 2, 3, 4, 5, 1, 6, 7, 8},  // Duplicate: col[0]=1 and col[5]=1.
        .square = {0, 1, 2, 3, 4, 5, 6, 7, 8}
    };
    // Expect duplicate in col: function should return 0.
    assert(check_uniqueness(idx) == 0);
    printf("test_duplicate_cols passed.\n");
}

// Test 3: Duplicate squares.
void test_duplicate_squares(void) {
    struct indices idx = {
        .row = {0, 1, 2, 3, 4, 5, 6, 7, 8},
        .col = {0, 1, 2, 3, 4, 5, 6, 7, 8},
        .square = {9, 8, 7, 6, 5, 4, 3, 2, 2}  // Duplicate: square[7]=2 and square[8]=2.
    };
    // Expect duplicate in square: function should return 0.
    assert(check_uniqueness(idx) == 0);
    printf("test_duplicate_squares passed.\n");
}

// Test 4: All values are unique.
void test_unique(void) {
    struct indices idx = {
        .row = {1, 2, 3, 4, 5, 6, 7, 8, 9},
        .col = {9, 8, 7, 6, 5, 4, 3, 2, 1},
        .square = {1, 2, 3, 4, 5, 6, 7, 8, 9}
    };
    // Expect all entries to be unique: function should return 1.
    assert(check_uniqueness(idx) == 1);
    printf("test_unique passed.\n");
}


int categorize(int num) {
    if (num >= 0 && num <= 2) {
        return 0;
    } else if (num >= 3 && num <= 5) {
        return 1;
    } else if (num >= 6 && num <= 8) {
        return 2;
    }
    return -1; // Default return value for out-of-range numbers
}

struct UArray2* valid_board()
{
    int width = 9;
    int height = 9;

    struct UArray2 *sudokuBoard = UArray2_new(width, height, sizeof(int));

    for (int y = 0; y < UArray2_height(sudokuBoard); y++)
    {
        for (int x = 0; x < UArray2_width(sudokuBoard); x++)
        {
            int *cell = (int *)UArray2_at(sudokuBoard, x, y);
            *cell = ((x + 3 * (y % 3) + categorize(y)) % 9) + 1;
        }
    }
    return sudokuBoard;
}

void test_valid_sudoku()
{
    struct UArray2 *sudokuBoard = valid_board();

    // Print the Sudoku board to the console
    for (int y = 0; y < UArray2_height(sudokuBoard); y++)
    {
        for (int x = 0; x < UArray2_width(sudokuBoard); x++)
        {
            int *cell = (int *)UArray2_at(sudokuBoard, x, y);
            printf("%d ", *cell);
        }
        printf("\n");
    }

    assert (isValidSudoku(sudokuBoard) == 1);

    UArray2_free(sudokuBoard);

    printf("Assertion sucessful\n");
}

void test_invalid_sudoku_col()
{
    struct UArray2 *sudokuBoard = valid_board();

    int *cell = (int *)UArray2_at(sudokuBoard, 6, 7);
    *cell = 7; // Two sevens in the same col

    // Print the Sudoku board to the console
    for (int y = 0; y < UArray2_height(sudokuBoard); y++)
    {
        for (int x = 0; x < UArray2_width(sudokuBoard); x++)
        {
            int *cell = (int *)UArray2_at(sudokuBoard, x, y);
            printf("%d ", *cell);
        }
        printf("\n");
    }

    assert (isValidSudoku(sudokuBoard) == 0);

    UArray2_free(sudokuBoard);

    printf("Assertion sucessful\n");
}


void test_invalid_sudoku_row()
{
    struct UArray2 *sudokuBoard = valid_board();

    int *cell = (int *)UArray2_at(sudokuBoard, 3, 4);
    *cell = 4; // Two fours in the same row

    // Print the Sudoku board to the console
    for (int y = 0; y < UArray2_height(sudokuBoard); y++)
    {
        for (int x = 0; x < UArray2_width(sudokuBoard); x++)
        {
            int *cell = (int *)UArray2_at(sudokuBoard, x, y);
            printf("%d ", *cell);
        }
        printf("\n");
    }

    assert (isValidSudoku(sudokuBoard) == 0);

    UArray2_free(sudokuBoard);

    printf("Assertion sucessful\n");
}

void test_invalid_sudoku_square()
{
    struct UArray2 *sudokuBoard = valid_board();

    int *cell = (int *)UArray2_at(sudokuBoard, 7, 7);
    *cell = 9; // Two nines in the same square

    // Print the Sudoku board to the console
    for (int y = 0; y < UArray2_height(sudokuBoard); y++)
    {
        for (int x = 0; x < UArray2_width(sudokuBoard); x++)
        {
            int *cell = (int *)UArray2_at(sudokuBoard, x, y);
            printf("%d ", *cell);
        }
        printf("\n");
    }

    assert (isValidSudoku(sudokuBoard) == 0);

    UArray2_free(sudokuBoard);

    printf("Assertion sucessful\n");
}
