#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#include "uarray.h"


extern struct UArray2 *UArray2_new(int width, int height, int size);
extern void UArray2_free(struct UArray2* matrix);
extern void *UArray2_at(struct UArray2 *matrix, int x, int y);
extern int UArray2_height(struct UArray2 *matrix);
extern int UArray2_width(struct UArray2 *matrix);
extern int UArray2_size(struct UArray2 *matrix);

int return_index(int x, int y, int width);

#endif