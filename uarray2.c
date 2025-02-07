#include <stdlib.h>
#include <stdio.h>
#include "uarray.h"
#include "uarray2.h"


struct UArray2 {
    UArray_T Umatrix;
    int width;
    int height;
};

int return_index(int x, int y, int width)
{
    return y * width + x;
}

struct UArray2 *UArray2_new(int width, int height, int size)
{
    struct UArray2* matrix = malloc(sizeof(struct UArray2));

    if (matrix == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for matrix");
        exit(1);
    }

    matrix->Umatrix = UArray_new(width * height, size);
    matrix->width = width;
    matrix->height = height;

    return matrix;
}

void UArray2_free(struct UArray2* matrix)
{
    if (matrix != NULL) {
        UArray_free(&matrix->Umatrix);
        free(matrix);
    }
}


void *UArray2_at(struct UArray2 *matrix, int x, int y)
{
    return UArray_at(matrix->Umatrix, return_index(x, y, matrix->width));
}

int UArray2_height(struct UArray2 *matrix)
{
    return matrix->height;
}


int UArray2_width(struct UArray2 *matrix)
{
    return matrix->width;
}


int UArray2_size(struct UArray2 *matrix)
{
    return UArray_size(matrix->Umatrix);
}


