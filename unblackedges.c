/*
 *     unblackedges.c
 *     Darius ___, Evren Uluer, 
 *     1/28/25
 *     unblackedges
 *
 *     This program can take in a pbm file and remove all of the
 *     edge connected black bits
 */

#include "bit2.h"
#include <stdlib.h>
#include <stdio.h>
#include "assert.h"
#include "pnmrdr.h"
#include "stack.h"

// These are entries of black pixels to put in a stack.
typedef struct {
        int row, col;
} BlackPixels;


Bit2_T pbmread(FILE *inputfp);
void   pbmwrite(Bit2_T bitmap);
void   swap_color(Stack_T pixels, Bit2_T bitmap);
void   push_pixels(Stack_T pixels, Bit2_T bitmap, int row, int col);
void   unblackedges(Bit2_T bitmap);
void   edgepix_to_stack(Stack_T pixels, Bit2_T bitmap, int row, int col);

/*
*  name:        main
*  purpose:     Reads a PBM file, removes edge-connected black pixels, 
*               and outputs the modified bitmap.
*  arguments:   An integer argc representing the number of command-line arguments,
*               and an array of character pointers argv representing 
*               the arguments.
*  return type: Integer (EXIT_SUCCESS or EXIT_FAILURE).
*  effect:      - Reads a PBM file from a given filename or standard input.
*               - Modifies the bitmap by removing edge-connected black pixels.
*               - Outputs the modified bitmap in PBM format to stdout.
*               - Allocates memory for the bitmap, which is freed before exiting.
*  expects:     - argc is either 1 (read from stdin) or 2 (read from file).
*               - If argc == 2, argv[1] must be a valid PBM file path.
*               - The PBM file must be properly formatted.
*               - If too many arguments are provided, the program exits with an error.
*/
int main(int argc, char *argv[])
{   
        if (argc == 2) { //read from a file   
                FILE *inputfp = fopen(argv[1], "r");
                assert(inputfp != NULL);

                Bit2_T bitmap = pbmread(inputfp);
                assert(bitmap != NULL);
                fclose(inputfp);
                unblackedges(bitmap);
                pbmwrite(bitmap);
                Bit2_free(&bitmap); // free         
        } else if (argc == 1) { //read from standard input
                Bit2_T bitmap = pbmread(stdin);
                assert(bitmap != NULL);
                unblackedges(bitmap);
                pbmwrite(bitmap);
                Bit2_free(&bitmap); // free
        } else {
                // there should only ever be max two arguments
                printf("Too many arguments\n");
                exit(EXIT_FAILURE);
        }
        
        return EXIT_SUCCESS;
}

/*
*  name:        pbmread
*  purpose:     Reads a PBM file and stores it as a 2D bit map.
*  arguments:   A FILE pointer representing the input PBM file.
*  return type: Bit2_T (a 2D bit map).
*  effect:      Allocates memory for a Bit2_T structure that must be freed
*               later.
*  expects:     The input file pointer is valid and points to a properly 
*               formatted PBM file.
*/
Bit2_T pbmread(FILE *inputfp)
{
        assert(inputfp != NULL);

        // get the information of the pbm by using pnmrdr
        Pnmrdr_T rdr = Pnmrdr_new(inputfp);
        Pnmrdr_mapdata map = Pnmrdr_data(rdr);

        Bit2_T bitmap = Bit2_new(map.height, map.width);

        // make sure to read the whole file
        for (unsigned row = 0; row < map.height; row++) {
                for (unsigned col = 0; col < map.width; col++) {
                    int bit = Pnmrdr_get(rdr);
                    Bit2_put(bitmap, row, col, bit);
                }
        }

        Pnmrdr_free(&rdr);
        return bitmap;
}

/*
*  name:        swap_color
*  purpose:     Iterates through a stack of black pixels and changes them
*               to white (0), ensuring all connected black pixels are also
*               processed.
*  arguments:   A stack containing black pixels to process and a bitmap
*               representing the 2D bit array.
*  return type: None.
*  effect:      Iterates through the stack and changes black pixels (1) to
*               white (0) in the bit map. Pushes adjacent black pixels to
*               the stack for further processing. Frees dynamically allocated
*               memory for each processed pixel, since they are allocated
*               structs.
*  expects:     Bitmap and pixels are not NULL and the pixels in the stack
*               contain valid row and column indices.
*/
void swap_color(Stack_T pixels, Bit2_T bitmap)
{
        assert(bitmap != NULL && pixels != NULL);
    
        while (!Stack_empty(pixels)) {
                BlackPixels *curr_pixel = Stack_pop(pixels);
                int row  = curr_pixel->row;
                int col = curr_pixel->col;
                free(curr_pixel);

                if (Bit2_get(bitmap, row, col) == 1) {  
                        Bit2_put(bitmap, row, col, 0);
                        push_pixels(pixels, bitmap, row, col);
                }
        }
    
}

/*
*  name:        unblackedges
*  purpose:     Iteratively removes black pixels (1s) that are connected 
*               to the edges of the bitmap using a stack.
*  arguments:   A bitmap representing the 2D bit array.
*  return type: None.
*  effect:      Allocates memory for a new stack and then frees it
*               at the end. Calls other functions that allocate memory, but
*               that will also be freed in their respective functions.
*  expects:     The bitmap pointer is not NULL.
*/
void unblackedges(Bit2_T bitmap)
{
        assert(bitmap != NULL);
        Stack_T pixels = Stack_new(); // create the stack
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        // top and bottom
        for (int col = 0; col < width; col++) {
                edgepix_to_stack(pixels, bitmap, 0, col);
                edgepix_to_stack(pixels, bitmap, height - 1, col);
        }

        // sides no overlap with the corners
        for (int row = 1; row < height - 1; row++) {
                edgepix_to_stack(pixels, bitmap, row, 0);
                edgepix_to_stack(pixels, bitmap, row, width - 1);
        }

        swap_color(pixels, bitmap); // process our stack
        Stack_free(&pixels); // free the stack
}

/*
*  name:        pbmwrite
*  purpose:     Writes a 2D bit map in PBM format to standard output.
*  arguments:   A Bit2_T representing the bitmap.
*  return type: None.
*  effect:      Prints the PBM representation of the bitmap to stdout.
*  expects:     The bitmap pointer is not NULL.
*/
void pbmwrite(Bit2_T bitmap)
{
        assert(bitmap != NULL);
        int width = Bit2_width(bitmap);
        int height = Bit2_height(bitmap);

        // print the header
        printf("P1\n");
        printf("%d %d\n", width, height);

        // print the data
        for (int row = 0; row < height; row++) {
                for (int col = 0; col < width; col++) {
                        printf("%d ", Bit2_get(bitmap, row, col));
                }

                printf("\n");
        }
}

/*
*  name:        push_pixels
*  purpose:     Identifies the four adjacent pixels (up, down, left, right) 
*               and pushes them onto the stack for processing.
*  arguments:   A stack containing black pixels to process and a bitmap
*               representing the 2D bit array, as well as two ints
*               represnting an index in the bitmap.
*  return type: None.
*  effect:      Dynamically allocates memory for up to four BlackPixels
*               structs, then pushes those onto a stack. If one is invalid
*               it is freed, otherwise the clients need to call swap_color
*               to free the rest in the stack.
*  expects:     Pixels and bitmap are not NULL. and row + col are valid bounds
*/
void push_pixels(Stack_T pixels, Bit2_T bitmap, int row, int col)
{
        assert(pixels != NULL && bitmap != NULL);

        BlackPixels *connected[4];
        for (int i = 0; i < 4; i++) {
                connected[i] = malloc(sizeof(BlackPixels));
                assert(connected[i] != NULL);
        }

        connected[0]->row = row - 1; // up
        connected[0]->col = col; // up
        connected[1]->row = row + 1; // down
        connected[1]->col = col;  // down
        connected[2]->row = row; // left
        connected[2]->col = col - 1; // left
        connected[3]->row = row; // right
        connected[3]->col = col + 1; // right

        for (int i = 0; i < 4; i++) { // make sure they're in bounds
                if (valid_index(bitmap, connected[i]->row, connected[i]->col)) {
                        Stack_push(pixels, connected[i]);
                } else {
                        free(connected[i]); // free if not
                }
        }
}

/*
*  name:        edgepix_to_stack
*  purpose:     Checks if a given edge pixel is black (1) and pushes
*               it onto the stack.
*  arguments:   A stack containing black pixels to process and a bitmap
*               representing the 2D bit array, as well as two ints
*               represnting an index in the bitmap.
*  return type: None.
*  effect:      Dynamically allocates space fro BlackPixel structs if
*               an edge is black and pushes it onto a stack. These
*               will be freed when swap_color is called.
*  expects:     Pixels and bitmap are not NULL. and row + col are valid bounds
*/
void edgepix_to_stack(Stack_T pixels, Bit2_T bitmap, int row, int col)
{
        if (Bit2_get(bitmap, row, col) == 1) {
                BlackPixels *curr_pixel = malloc(sizeof(BlackPixels));
                curr_pixel->row = row;
                curr_pixel->col = col;
                Stack_push(pixels, curr_pixel);
        }
}