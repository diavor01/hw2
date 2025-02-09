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


Bit2_T pbmread(FILE *inputfp);
void   pbmwrite(Bit2_T bitmap);
void   swap(Bit2_T bitmap, int row, int col);
void   edges(Bit2_T bitmap);
void   unblackedges(Bit2_T bitmap, int row, int col);

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
        edges(bitmap);
        pbmwrite(bitmap);
        Bit2_free(&bitmap); // free         
    } else if (argc == 1) { //read from standard input
        Bit2_T bitmap = pbmread(stdin);
        assert(bitmap != NULL);
        edges(bitmap);
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
    assert(input != NULL);

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

    return bitmap;
}

/*
*  name:        swap
*  purpose:     Sets the bit at a specified row and column to 0.
*  arguments:   A Bit2_T representing the bitmap, an integer row index, 
*               and an integer column index.
*  return type: None.
*  effect:      Modifies the bit at the specified position by setting it to 0.
*  expects:     The bitmap pointer is not NULL, and row/column indices 
*               are within valid bounds.
*/
void swap(Bit2_T bitmap, int row, int col)
{
    assert(bitmap != NULL);
    assert(valid_index(bitmap, row, col));
    Bit2_put(bitmap, row, col, 0);
}

/*
*  name:        edges
*  purpose:     Identifies black pixels (1s) on the edges of the bitmap 
*               and removes them by calling unblackedges.
*  arguments:   A Bit2_T representing the bitmap.
*  return type: None.
*  effect:      Modifies the bitmap by changing edge-connected black pixels
*               to white (0).
*  expects:     The bitmap pointer is not NULL.
*/
void edges(Bit2_T bitmap)
{
    assert(bitmap != NULL);
    int height = Bit2_height(bitmap);
    int width = Bit2_width(bitmap);

    for (int col = 0; col < width; col++) { // top edge
        if (Bit2_get(bitmap, 0, col) == 1) {
            unblackedges(bitmap, 0, col);
        }
    }

    for (int col = 0; col < width; col++) { // bottom edge
        if (Bit2_get(bitmap, height - 1, col) == 1) {
            unblackedges(bitmap, height - 1, col);
        }
    }

    for (int row = 1; row < height - 1; row++) { // left edge
        if (Bit2_get(bitmap, row, 0) == 1) {
            unblackedges(bitmap, row, 0);
        }
    }

    for (int row = 1; row < height - 1; row++) { // right edge
        if (Bit2_get(bitmap, row, width - 1) == 1) {
            unblackedges(bitmap, row, width - 1);
        }
    }
}

/*
*  name:        unblackedges
*  purpose:     Recursively removes black pixels (1s) that are connected 
*               to the edges of the bitmap.
*  arguments:   A Bit2_T representing the bitmap, an integer row index, 
*               and an integer column index.
*  return type: None.
*  effect:      Modifies the bitmap by turning black edge-connected pixels into white (0).
*  expects:     The bitmap pointer is not NULL, and row/column indices are valid.
*/
void unblackedges(Bit2_T bitmap, int row, int col)
{
    assert(bitmap != NULL);    

    if (valid_index(bitmap, row, col)) {
        if (Bit2_get(bitmap, row, col) == 1) {
            swap(bitmap, row, col);

            // use unblackedges on down, up, left, right
            unblackedges(bitmap, row - 1, col);
            unblackedges(bitmap, row + 1, col);
            unblackedges(bitmap, row, col - 1);
            unblackedges(bitmap, row, col + 1);
        }
    }
}

/*
*  name:        pbmwrite
*  purpose:     Writes a 2D bit map in PBM format to standard output.
*  arguments:   A Bit2_T representing the bitmap.
*  return type: None.
*  effect:      Prints the PBM representation of the bitmap to stdout.
*  expects:     The bitmap pointer is not NULL.
*/
void pbmwrite(Bit2_T bitmap){
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