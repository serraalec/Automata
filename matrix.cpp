
/* ========================================================================
   $File: matrix.cpp$
   $Date: 5/22/2020$
   $Revision: 1$
   $Creator: Alec Serra $
    ======================================================================== */

#include "matrix.h"

void mult_r32_vec_2(R32 a, Vec_2 &in)
{
	in.I *= a;
	in.J *= a;
}

void add_r32_vec_2(R32 a, Vec_2 &in)
{
	in.I += a;
	in.J += a;
}

void mult_r32_vec_3(R32 a, Vec_3 &in)
{
	in.I *= a;
	in.J *= a;
	in.K *= a;
}

void norm_Vec_3(Vec_3 &in)
{
	R32 sqr  = sqrtf(in.I*in.I + in.J*in.J + in.K*in.K);
	
	in.I /= sqr;
	in.J /= sqr;
	in.K /= sqr;
	
}

void norm_vec_2(Vec_2 &in)
{
	R32 sqr  = sqrtf(in.I*in.I + in.J*in.J);
	if(sqr)
	{
		in.I /= sqr;
		in.J /= sqr;
	}
}

void add_Vec_3(Vec_3 &X, Vec_3 &Y, Vec_3 &out)
{
	out.I = X.I + Y.I;
	out.J = X.J + Y.J;
	out.K = X.K + Y.K;
}

void add_vec_2(Vec_2 &X, Vec_2 &Y, Vec_2 &out)
{
	out.I = X.I + Y.I;
	out.J = X.J + Y.J;
}
void sub_vec_2(Vec_2 &X, Vec_2 &Y, Vec_2 &out)
{
	out.I = X.I - Y.I;
	out.J = X.J - Y.J;
}

void mult_vec_2_vec_2(Vec_2 &X, Vec_2 &Y, Vec_2 &out)
{
	out.I = X.I * Y.I;
	out.J = X.J * Y.J;
}

void mult_vec_2_m_2x2(Vec_2 &in, Vec_2 &out, Matrix_2x2 &m)
{
	out.I = m.M[0][0]*in.I + m.M[0][1]*in.J;
	out.J = m.M[1][0]*in.I + m.M[1][1]*in.J;
}

void mult_Vec_3_m_4x4(Vec_3 &in, Vec_3 &out, Matrix_4x4 &m)
{
	out.I = in.I * m.M[0][0] + in.J * m.M[1][0] + in.K * m.M[2][0] + m.M[3][0];
	out.J = in.I * m.M[0][1] + in.J * m.M[1][1] + in.K * m.M[2][1] + m.M[3][1];
	out.K = in.I * m.M[0][2] + in.J * m.M[1][2] + in.K * m.M[2][2] + m.M[3][2];
	R32 w = in.I * m.M[0][3] + in.J * m.M[1][3] + in.K * m.M[2][3] + m.M[3][3];

	if (w != 0.0f)
	{
		out.I /= w; out.J /= w; out.K /= w;
	}
}

