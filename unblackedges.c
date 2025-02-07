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

int main(int argc, char *argv[])
{   
    if (argc == 2) 
    {        
        FILE *inputfp = fopen(argv[1], "r");
        assert(inputfp != NULL);

        Bit2_T bitmap = pbmread(inputfp);
        assert(bitmap != NULL);
        fclose(inputfp);
        edges(bitmap);
        pbmwrite(bitmap);
        Bit2_free(&bitmap);             
    } 
    else if (argc == 1)
    {
        Bit2_T bitmap = pbmread(stdin);
        assert(bitmap != NULL);
        edges(bitmap);
        pbmwrite(bitmap);
        Bit2_free(&bitmap);
    }
    else 
    {
        printf("Too many arguments\n");
        exit(1);
    }
        
    return EXIT_SUCCESS;
}

Bit2_T pbmread(FILE *inputfp)
{
    Pnmrdr_T rdr = Pnmrdr_new(inputfp);
    Pnmrdr_mapdata map = Pnmrdr_data(rdr);

    Bit2_T bitmap = Bit2_new(map.height, map.width);

    for (unsigned row = 0; row < map.height; row++) {
        for (unsigned col = 0; col < map.width; col++) {
            int bit = Pnmrdr_get(rdr);
            Bit2_put(bitmap, row, col, bit);
        }
    }

    return bitmap;
}

void swap(Bit2_T bitmap, int row, int col)
{
    Bit2_put(bitmap, row, col, 0);
}

void edges(Bit2_T bitmap)
{
    int height = Bit2_height(bitmap);
    int width = Bit2_width(bitmap);

    for (int col = 0; col < width; col++) {
        if (Bit2_get(bitmap, 0, col) == 1) {
            unblackedges(bitmap, 0, col);
        }
    }

    for (int col = 0; col < width; col++) {
        if (Bit2_get(bitmap, height - 1, col) == 1) {
            unblackedges(bitmap, height - 1, col);
        }
    }

    for (int row = 1; row < height - 1; row++) {
        if (Bit2_get(bitmap, row, 0) == 1) {
            unblackedges(bitmap, row, 0);
        }
    }

    for (int row = 1; row < height - 1; row++) {
        if (Bit2_get(bitmap, row, width - 1) == 1) {
            unblackedges(bitmap, row, width - 1);
        }
    }
}

void unblackedges(Bit2_T bitmap, int row, int col)
{
    if (valid_index(bitmap, row, col))
    {
        if (Bit2_get(bitmap, row, col) == 1)
        {
            swap(bitmap, row, col);

            unblackedges(bitmap, row - 1, col);
            unblackedges(bitmap, row + 1, col);
            unblackedges(bitmap, row, col - 1);
            unblackedges(bitmap, row, col + 1);
        }
    }
}

void pbmwrite(Bit2_T bitmap){
    int width = Bit2_width(bitmap);
    int height = Bit2_height(bitmap);

    printf("P1\n");
    printf("%d %d\n", width, height);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            printf("%d ", Bit2_get(bitmap, row, col));
        }
        printf("\n");
    }
}