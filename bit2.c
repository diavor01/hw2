/*
 *     bit2.c
 *     Darius-Stefan Iavorschi, Evren Uluer, 
 *     1/28/25
 *     bit2
 *
 *     This program should be able to hold a 2D bit array that
 *     represents a pbm file
 */

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "bit2.h"
#include "bit.h"

/* This struct represents a 2D bitmap using a 1D bit array from Hanson */
struct Bit2_T {
        int rows;
        int cols;
        Bit_T bit_arr;
};

/* 
*  name:        Bit2_new
*  purpose:     This function initializes and llocates space for a 2D bit map.
*  arguments:   This takes in two integers, one specifies the number of rows
*               wanted, and the other specifies the number of columns
*  return type: Pointer to a 2D bit map
*  effect:      This function allocates memory so the user needs to free it 
*               later.
*  expects:     The arguments must be greater than zero or else an assert
*               fails, meaning there is a runtime error.
*/
Bit2_T Bit2_new(int rows, int cols) 
{
        assert(rows > 0 && cols > 0);
        Bit2_T bit2 = malloc(sizeof(*bit2)); // malloc space
        assert(bit2 != NULL);
        bit2->bit_arr = Bit_new(rows * cols); // use hanson bit
        assert( bit2->bit_arr != NULL);
        bit2->rows = rows;
        bit2->cols = cols;

        return bit2;
}

/*
*  name:        Bit2_put
*  purpose:     Stores a bit at the specified row and column in the 2D bit 
*               map.
*  arguments:   A Bit2_T representing the bitmap, an integer row index, 
*               an integer column index, and an integer bit (0 or 1) to store.
*  return type: The previous bit value at the given row and column.
*  effect:      Modifies the bit stored in the given position of the bitmap.
*  expects:     The bitmap pointer is not NULL, row and column indices 
*               are within valid bounds.
*/
int Bit2_put(Bit2_T bit2, int row, int col, int bit)
{
        assert(bit2 != NULL);
        // make sure bounds are valid
        assert(valid_index(bit2, row, col));
        int index = row * bit2->cols + col;
        int last_bit = Bit_put(bit2->bit_arr, index, bit);
    
        return last_bit;
}

/*
*  name:        Bit2_get
*  purpose:     Retrieves the bit stored at the specified row and column.
*  arguments:   A Bit2_T representing the bitmap, an integer row index, 
*               and an integer column index.
*  return type: The bit value (0 or 1) stored at the given row and column.
*  effect:      None
*  expects:     The bitmap pointer is not NULL, row and column indices 
*               are within valid bounds.
*/
int Bit2_get(Bit2_T bit2, int row, int col)
{
        assert(bit2 != NULL);
        // make sure bounds are valid
        assert(valid_index(bit2, row, col));
        int index = row * bit2->cols + col;
        int curr_bit = Bit_get(bit2->bit_arr, index);
    
        return curr_bit;
}

/*
*  name:        Bit2_free
*  purpose:     Frees all allocated memory associated with the bitmap.
*  arguments:   A pointer to a Bit2_T (Bit2_T *) that will be freed.
*  return type: None.
*  effect:      Deallocates memory, sets the pointer to NULL to prevent 
*               use-after-free errors.
*  expects:     The pointer to Bit2_T is not NULL, and it contains 
*               a valid allocated bitmap.
*/
void Bit2_free(Bit2_T *bit2)
{
        assert(bit2 != NULL || *bit2 != NULL);
        if ((*bit2)->bit_arr != NULL) { // check before freeing
                Bit_free(&(*bit2)->bit_arr);
        }

        free(*bit2);
        *bit2 = NULL;
}

/*
*  name:        Bit2_width
*  purpose:     Returns the number of columns in the bitmap.
*  arguments:   A Bit2_T representing the bitmap.
*  return type: Integer (number of columns).
*  effect:      None.
*  expects:     The bitmap pointer is not NULL.
*/
int Bit2_width(Bit2_T bit2)
{
        assert(bit2 != NULL);
        return bit2->cols;
}

/*
*  name:        Bit2_height
*  purpose:     Returns the number of rows in the bitmap.
*  arguments:   A Bit2_T representing the bitmap.
*  return type: Integer.
*  effect:      None.
*  expects:     The bitmap pointer is not NULL.
*/
int Bit2_height(Bit2_T bit2)
{
        assert(bit2 != NULL);
        return bit2->rows;
}

/*
*  name:        Bit2_map_row_major
*  purpose:     Applies a given function to each element in the bitmap, 
*               iterating in row-major order (row by row).
*  arguments:   A Bit2_T representing the bitmap, a function pointer apply
*               that takes row, column, bitmap, value, and a closure pointer, 
*               and a void pointer cl for additional arguments.
*  return type: None.
*  effect:      Calls apply on each bit in row-major order.
*  expects:     The bitmap pointer is not NULL, and apply is a valid function.
*/
void Bit2_map_row_major(Bit2_T bit2, void apply(
    int row, int col, Bit2_T bit2, int value, void *cl), void *cl) 
{
        assert(bit2 != NULL);
        for (int r = 0; r < bit2->rows; r++) {
                for (int c = 0; c < bit2->cols; c++) {
                        int value = Bit2_get(bit2, r, c);
                        // pointer to a function
                        apply(r, c, bit2, value, cl);
                }
        }
}

/*
*  name:        Bit2_map_col_major
*  purpose:     Applies a given function to each element in the bitmap, 
*               iterating in column-major order (column by column).
*  arguments:   A Bit2_T representing the bitmap, a function pointer apply
*               that takes row, column, bitmap, value, and a closure pointer, 
*               and a void pointer cl for additional arguments.
*  return type: None.
*  effect:      Calls apply on each bit in column-major order.
*  expects:     The bitmap pointer is not NULL, and apply is a valid function.
*/
void Bit2_map_col_major(Bit2_T bit2, void apply(
    int row, int col, Bit2_T bit2, int value, void *cl), void *cl) 
{
        assert(bit2 != NULL);
        for (int c = 0; c < bit2->cols; c++) {
                for (int r = 0; r < bit2->rows; r++) {
                        int value = Bit2_get(bit2, r, c);
                        // pointer to a function
                        apply(r, c, bit2, value, cl);
                }
        }
}

/*
*  name:        valid_index
*  purpose:     Checks whether a given row and column index are within the 
*               valid bounds of the bitmap.
*  arguments:   A Bit2_T representing the bitmap, an integer row index, 
*               and an integer column index.
*  return type: Integer (boolean-ish: 1 if valid, 0 if invalid).
*  effect:      None.
*  expects:     The bitmap pointer is not NULL.
*/
int valid_index(Bit2_T bit2, int row, int col)
{
        assert(bit2 != NULL);
        return (row >= 0 && row < Bit2_height(bit2) &&
                col >= 0 && col < Bit2_width(bit2));
}
