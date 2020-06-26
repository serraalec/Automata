#ifndef MATRIX_H
#define MATRIX_H

/* ========================================================================
   $File: matrix.h $
   $Date: 5/22/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */

#include "types.h"

struct Vec_3
{
	R32 I;
	R32 J;
	R32 K;
};
struct Vec_2
{
	R32 I;
	R32 J;
};

struct Matrix_2x2
{
	R32 M[2][2];
};

struct Matrix_3x3
{
	R32 M[3][3];
};
struct Matrix_4x4
{
	R32 M[4][4];
};



#endif