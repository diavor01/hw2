/*
 *     sudoku.c
 *     Darius-Stefan Iavorschi, Evren Uluer
 *     1/28/25
 *     sudoku
 *
 *     This program takes in a PGM file in order to check if the pixels
 *     represent a valid sudoku board. The pixels are stored inside a UArray2
 *     matrix.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "except.h"
#include "uarray2.h"
#include "pnmrdr.h"

/*  Every digit in the sudoku puzzle must have the following properties: be in
    every possible row, column and box without repetition. Thus, we must keep
    track of each digit's position and search for duplicate.
*   Examples: for digit 2, we can have the following data:
    struct digit idx = {
        .row = {1, 2, 3, 4, 5, 6, 7, 8, 9},
        .col = {9, 8, 7, 6, 5, 4, 3, 2, 1},
        .box = {1, 2, 3, 4, 5, 6, 7, 8, 9}
    };
    The positions of 2 are valid for a sudoku solution.

*   On the other hand,
    struct digit idx = {
        .row = {0, 1, 2, 3, 4, 5, 6, 7, 8},
        .col = {0, 1, 2, 3, 4, 5, 6, 7, 8},
        .box = {9, 8, 7, 6, 5, 4, 3, 2, 2}  Duplicate: box[7]=2 and box[8]=2.
    };
    is invalid because there are duplicates inside the box array.
*/
struct digit
{
    int row[10];
    int col[10];
    int box[10];
};

/*
*  name:        return_box
*  purpose:     Calculate 3x3 box number (1-9) for given Sudoku cell coordinates
*  arguments:   int col (0-8 column index), int row (0-8 row index)
*  return type: int
*  effect:      Implements box numbering per Sudoku 3x3 grid layout:
                1 2 3
                4 5 6
                7 8 9
*  expects:     0 ≤ col < 9, 0 ≤ row < 9. Violation produces invalid box numbers
*/
int return_box(int col, int row)
{
    return ((row / 3) * 3) + (col / 3) + 1;
}

/*
*  name:        check_uniqueness
*  purpose:     Verify a digit's positions satisfy Sudoku uniqueness constraints
*  arguments:   struct digit (contains row/col/box positions for a single digit)
*  return type: bool
*  effect:      Checks all position pairs for duplicates in any dimension
*  expects:     Fully populated struct digit with 9 positions. Returns false on
                any duplicate in row, column, or box dimensions
*/
bool check_uniqueness(struct digit value)
{
        for (int i = 0; i < 8; i++) {
                for (int j = i + 1; j < 9; j++) {
                        if (value.row[j] == value.row[i] || 
                            value.col[j] == value.col[i] ||
                            value.box[j] == value.box[i]) {
                                return false;
                        }
                }
    }

    return true;
}

bool contruct_digits_properties(UArray2_T matrix, 
                                struct digit *digits_properties)
{
        int size[10] = {0};
        for (int i = 1; i < 10; i++) {
                for (int j = 1; j < 10; j++) {
                        int value = *(int *)UArray2_at(matrix, j - 1, i - 1);

                        /*Making sure the value is a valid sudoku digit*/
                        if (value > 0 && value < 10) {
                                int s = size[value];
                                digits_properties[value].row[s] = i;
                                digits_properties[value].col[s] = j;
                                digits_properties[value].box[s] = return_box(j - 1, i - 1);
                                size[value]++;
                        }
                        else
                        {
                                fprintf(stderr, "The digit is inavlid "
                                    "(not between 1 and 9)\n");
                                free(digits_properties);
                                return false;
                        }
                }
        }

        return true;
}

/*
*  name:        is_valid_sudoku
*  purpose:     Validate complete Sudoku solution against game rules
*  arguments:   UArray2_T matrix (9x9 grid)
*  return type: bool
*  effect:      Builds position tracking structs for all digits, then verifies
                uniqueness in all dimensions via check_uniqueness()
*  expects:     9x9 matrix containing only 1-9 values. Terminates program on:
                - NULL matrix
                - Invalid digit values (outside 1-9)
                - Memory allocation failures
*/
bool is_valid_sudoku(UArray2_T matrix)
{
        if (matrix == NULL) {
                perror(
                "Failed to allocate memory for " 
                "the matrix inside is_valid_sudoku");
                exit(1);
        }

        struct digit *digits_properties = malloc(10 * sizeof(struct digit));


        if (!contruct_digits_properties(matrix, digits_properties))
        { return false; }

        bool is_valid = true;
        for (int i = 1; i < 10; i++) {
                if (!check_uniqueness(digits_properties[i]))
                {
                        is_valid = false;
                        break;
                }
        }

        free(digits_properties);
        return is_valid;
}

/*
 *  name:        cleanup
 *  purpose:     Release all resources used during Sudoku validation process
 *  arguments:   Pnmrdr_T reader (PNM reader instance), FILE* file 
 *               (input source), bool file_provided (flag for file vs stdin), 
 *               UArray2_T matrix (Sudoku grid)
 *  return type: void
 *  effect:      Frees PNM reader, closes file (if provided), and deallocates 
 *               matrix. Handles NULL pointers safely.
 *  expects:     All parameters may be NULL except file_provided.
 */
void cleanup(Pnmrdr_T reader, FILE* file, bool file_provided, UArray2_T matrix)
{
        if (reader) { Pnmrdr_free(&reader); }
        if (file_provided && file != NULL) { fclose(file); }
        if (matrix) {UArray2_free(&matrix); }
}

/*
 *  name:        check_dimensions
 *  purpose:     Validate Sudoku grid dimensions are exactly 9x9
 *  arguments:   int width, int height (dimensions from PNM header)
 *  return type: bool
 *  effect:      Checks if width and height both equal 9. 
 *               Logs error on mismatch.
 *  expects:     Positive integer dimensions. Returns true only for 9x9 grids.
 */
bool check_dimensions(int width, int height)
{
        if (width != 9 || height != 9) {
                fprintf(stderr, "Invalid dimensions\n");
                return false;
        }
        return true;
}

/*
 *  name:        assign_values
 *  purpose:     Load pixel values from PNM reader into Sudoku grid matrix
 *  arguments:   int width, height (grid dimensions), UArray2_T matrix 
 *               (target grid), Pnmrdr_T reader (pixel source)
 *  return type: void
 *  effect:      Iterates row-wise through matrix, stores each pixel value as 
 *               integer. Assumes reader provides exactly width*height values.
 *  expects:     Properly initialized matrix matching dimensions, valid reader 
 *               with sufficient pixels.
 */
void assign_values(int width, int height, UArray2_T matrix, Pnmrdr_T reader)
{
        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        int pixel = Pnmrdr_get(reader);
                        *((int *)UArray2_at(matrix, col, row)) = pixel;
                }
        }
}

/*
 *  name:        check_input
 *  purpose:     Initialize PNM reader from appropriate input source 
 *               (file/stdin)
 *  arguments:   Pnmrdr_T *pointer_reader (output for reader instance), 
 *               FILE *file (input handle), bool file_provided (source flag)
 *  return type: bool
 *  effect:      Creates reader from file or stdin based on file_provided. 
 *               Handles NULL file errors when file expected.
 *  expects:     Valid file pointer if file_provided=true. Returns false on 
 *               invalid file state, true otherwise.
 */
bool check_input(Pnmrdr_T *pointer_reader, FILE *file, bool file_provided)
{
        if (file != NULL && file_provided) {
                *pointer_reader = Pnmrdr_new(file);
        } else {
                if (file == NULL && file_provided) {
                        fprintf(stderr, "FILE pointer is NULL despite "
                                "file_provided being true\n");
                        return false;
                } else {
                        *pointer_reader = Pnmrdr_new(stdin);
                }
        }

        return true;
}

/*
*  name:        read_input
*  purpose:     Read and validate Sudoku grid from PNM input source
*  arguments:   FILE* (input source), bool (indicates if file was provided)
*  return type: bool
*  effect:      - Reads PNM data using pnmrdr
                - Constructs 9x9 UArray2 from pixel values
                - Validates grid via is_valid_sudoku()
                - Exits program with appropriate status code
*  expects:     - Valid PGM format with 9x9 dimensions and 1-9 pixel values
                - Handles PNM_READER exceptions (Badformat, Count)
                - Closes file if provided, cleans up PNM reader resources
*/
bool read_input(FILE *file, bool file_provided)
{
        Pnmrdr_T reader = NULL;
        UArray2_T matrix = NULL;
        bool success = false;

        
        if (!check_input(&reader, file, file_provided)) {
                cleanup(reader, file, file_provided, matrix); 
                return success;
        }

        TRY {
                Pnmrdr_mapdata data = Pnmrdr_data(reader);
                int width = data.width;
                int height = data.height;

                /* Validate dimensions */
                if (!check_dimensions(data.width, data.height)) {
                        cleanup(reader, file, file_provided, matrix); 
                        return success;
                }

                matrix = UArray2_new(width, height, sizeof(int));

                assign_values(width, height, matrix, reader);

                /* Validate Sudoku */
                success = is_valid_sudoku(matrix);
        } EXCEPT(Pnmrdr_Badformat) {
                fprintf(stderr, "Invalid PNM format\n");
                success = false;
        } EXCEPT(Pnmrdr_Count) {
                fprintf(stderr, "Incorrect number of pixels\n");
                success = false;
        /*The FINALLY block ensures that resources are cleaned up regardless of
        whether an exception is raised or not*/
        } FINALLY {
                cleanup(reader, file, file_provided, matrix);

        }
        END_TRY;

        return success;
}

/*
*  name:        return_status
*  purpose:     A helper function to return the exit status of the program. It
                receives input from read_input
*  arguments:   bool
*  return type: void
*  effect:      - Exits program with validation status
*  expects:     - boolean value. True means a valid sudoku solution has been
                provided
*/
void return_status(bool exit_status)
{
        if (exit_status) {
                exit(EXIT_SUCCESS);
        }
        exit(EXIT_FAILURE);
}

/*
*  name:        main
*  purpose:     Entry point for Sudoku validation program
*  arguments:   int argc, char* argv[] (command-line arguments)
*  return type: int
*  effect:      - Parses command line arguments
                - Routes input source (file/stdin)
                - Initiates validation process via read_input()
                - Exits program with validation status
*  expects:     - argc == 1 (stdin input) or 2 (file input)
                - Valid file path when argc == 2
                - Terminates on invalid argument count or unopenable file
*/
int main(int argc, char *argv[])
{
        if (argc > 2) {
                perror("The syntax of the sudoku command is "
                       "./sudoku [ filename ]\n");
                exit(EXIT_FAILURE);
        }

        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");

                if (fp == NULL) {
                    perror("Error opening file");
                    exit(EXIT_FAILURE);
                }

                return_status(read_input(fp, true));
        } else {
                return_status(read_input(NULL, false));
        }

        return 0;
}

