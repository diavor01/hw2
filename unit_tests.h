#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "sudoku.h"
#include "uarray.h"
#include "uarray2.h"

/*Testing the return box function*/
void test_return_square()
{
        int x = 4;
        int y = 0;
        assert(2 == return_box(x, y));

        x = 0;
        y = 7;
        assert(7 == return_box(x, y));

        x = 8;
        y = 5;
        assert(6 == return_box(x, y));

        printf("Tests passed sucessfully\n");
}

/*Tests for the check_uniqueness function*/

/*Test 1: Duplicate rows*/
void test_duplicate_rows(void) {
        struct digit idx = {
                .row = {1, 1, 2, 3, 4, 5, 6, 7, 8}, /*Duplicate: 
                                                    row[0]=1 and row[1]=1*/
                .col = {0, 1, 2, 3, 4, 5, 6, 7, 8},
                .box = {0, 1, 2, 3, 4, 5, 6, 7, 8}
        };
        /*Expect duplicate in row: function should return 0*/
        assert(check_uniqueness(idx) == 0);
        printf("test_duplicate_rows passed.\n");
}

/*Test 2: Duplicate columns.*/
void test_duplicate_cols(void) {
        struct digit idx = {
            .row = {0, 1, 2, 3, 4, 5, 6, 7, 8},
            .col = {1, 2, 3, 4, 5, 1, 6, 7, 8}, /*Duplicate: 
                                                col[0]=1 and col[5]=1*/
            .box = {0, 1, 2, 3, 4, 5, 6, 7, 8}
        };
        /*Expect duplicate in col: function should return 0.*/
        assert(check_uniqueness(idx) == 0);
        printf("test_duplicate_cols passed.\n");
}

/*Test 3: Duplicate squares.*/
void test_duplicate_squares(void) {
        struct digit idx = {
                .row = {0, 1, 2, 3, 4, 5, 6, 7, 8},
                .col = {0, 1, 2, 3, 4, 5, 6, 7, 8},
                .box = {9, 8, 7, 6, 5, 4, 3, 2, 2} /*Duplicate: 
                                                    box[7]=2 and box[8]=2.*/
        };
        /*Expect duplicate in box: function should return 0.*/
        assert(check_uniqueness(idx) == 0);
        printf("test_duplicate_squares passed.\n");
}

/*Test 4: All values are unique.*/
void test_unique(void) {
        struct digit idx = {
                .row = {1, 2, 3, 4, 5, 6, 7, 8, 9},
                .col = {9, 8, 7, 6, 5, 4, 3, 2, 1},
                .box = {1, 2, 3, 4, 5, 6, 7, 8, 9}
        };
        /*Expect all entries to be unique: function should return 1.*/
        assert(check_uniqueness(idx) == 1);
        printf("test_unique passed.\n");
}

/*Helper function in order to create a valid sudoku board*/
int categorize(int num) {
        if (num >= 0 && num <= 2) {
                return 0;
        } else if (num >= 3 && num <= 5) {
                return 1;
        } else if (num >= 6 && num <= 8) {
                return 2;
        }
        return -1;
}

/*Another helper function used for testing*/
UArray2_T valid_board()
{
        int width = 9;
        int height = 9;

        UArray2_T sudokuBoard = UArray2_new(width, height, sizeof(int));

        for (int y = 0; y < UArray2_height(sudokuBoard); y++) {
                for (int x = 0; x < UArray2_width(sudokuBoard); x++) {
                        int *cell = (int *)UArray2_at(sudokuBoard, x, y);
                        *cell = ((x + 3 * (y % 3) + categorize(y)) % 9) + 1;
                }
        }
        return sudokuBoard;
    }

/*Yet another helper function in order to print the board to the console*/
void print_sudoku(UArray2_T sudokuBoard)
{
        for (int y = 0; y < UArray2_height(sudokuBoard); y++) {
                for (int x = 0; x < UArray2_width(sudokuBoard); x++) {
                    int *cell = (int *)UArray2_at(sudokuBoard, x, y);
                    printf("%d ", *cell);
                }
                printf("\n");
        }
}

/*Sudoku tests*/ 

/*Test 1: Test if the valid board is recognized appropriatedly*/ 
void test_valid_sudoku()
{
        UArray2_T sudokuBoard = valid_board(); 

        /*Print the Sudoku board to the console*/
        print_sudoku(sudokuBoard);

        assert(isValidSudoku(sudokuBoard) == 1);

        UArray2_free(&sudokuBoard);

        printf("Assertion sucessful\n");
}

/*Test 2: check invalid sudoku*/
void test_invalid_sudoku_col()
{
        UArray2_T sudokuBoard = valid_board();

        int *cell = (int *)UArray2_at(sudokuBoard, 6, 7);
        *cell = 7; /*Two sevens in the same col*/

        /*Print the Sudoku board to the console*/
        print_sudoku(sudokuBoard);

        assert (isValidSudoku(sudokuBoard) == 0);

        UArray2_free(&sudokuBoard);

        printf("Assertion sucessful\n");
}

/*Test 3: check invalid sudoku*/
void test_invalid_sudoku_row()
{
        UArray2_T sudokuBoard = valid_board();

        int *cell = (int *)UArray2_at(sudokuBoard, 3, 4);
        *cell = 4; /*Two fours in the same row*/

        /*Print the Sudoku board to the console*/
        print_sudoku(sudokuBoard);

        assert (isValidSudoku(sudokuBoard) == 0);

        UArray2_free(&sudokuBoard);

        printf("Assertion sucessful\n");
}

/*Test 4: check invalid sudoku*/
void test_invalid_sudoku_square()
{
        UArray2_T sudokuBoard = valid_board();

        int *cell = (int *)UArray2_at(sudokuBoard, 7, 7);
        *cell = 9; /*Two nines in the same box*/

        /*Print the Sudoku board to the console*/
        print_sudoku(sudokuBoard);

        assert (isValidSudoku(sudokuBoard) == 0);

        UArray2_free(&sudokuBoard);

        printf("Assertion sucessful\n");
}

/*Testing the UArray2 functions*/

/*Test 1*/
void test_UArray2_new() {
        int width = 5;
        int height = 3;
        int size = sizeof(int);
        UArray2_T matrix = UArray2_new(width, height, size);

        assert(matrix != NULL);
        assert(UArray2_width(matrix) == width);
        assert(UArray2_height(matrix) == height);
        assert(UArray2_size(matrix) == sizeof(int));

        /*Freeing the memory of UArray2 is tested implicitly*/
        UArray2_free(&matrix);
        assert(matrix == NULL);

        printf("Assertions sucessful\n");
}

/*Test 2*/
void test_UArray2_at() {
        int width = 3;
        int height = 4;
        int size = sizeof(int);
        UArray2_T matrix = UArray2_new(width, height, size);

        /*Set values using UArray2_at and check their positions*/
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int *elem = UArray2_at(matrix, x, y);
                *elem = y * width + x;
            }
        }

        /*Verify values via direct UArray access*/
        for (int i = 0; i < width * height; i++) {

            /*Note: UArray2_get_array does not exist in the final version of 
            UArray2 interface*/
            UArray_T UArray = UArray2_get_array(matrix);
            int *expected = UArray_at(UArray, i);
            int x = i % width;
            int y = i / width;
            int *actual = UArray2_at(matrix, x, y);
            assert(*expected == *actual);
            assert(expected == actual);
        }

        UArray2_free(&matrix);
}
