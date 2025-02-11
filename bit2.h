/*
 *     bit2.h
 *     Darius-Stefan Iavorschi, Evren Uluer, 
 *     1/28/25
 *     bit2
 *
 *     This file holds the interface for a bit map
 */

#ifndef BIT2_INCLUDED
#define BIT2_INCLUDED

typedef struct Bit2_T *Bit2_T;

extern Bit2_T Bit2_new(int rows, int cols);
extern int Bit2_put(Bit2_T bit2, int row, int col, int bit);
extern int Bit2_get(Bit2_T bit2, int row, int col);
extern void Bit2_free(Bit2_T *bit2);
extern int Bit2_width(Bit2_T bit2);
extern int Bit2_height(Bit2_T bit2);
extern void Bit2_map_row_major(Bit2_T bit2, void apply(
            int row, int col, Bit2_T bit2, int value, void *cl), void *cl);
extern void Bit2_map_col_major(Bit2_T bit2, void apply(
            int row, int col, Bit2_T bit2, int value, void *cl), void *cl);
int valid_index(Bit2_T bit2, int row, int col);

#endif
