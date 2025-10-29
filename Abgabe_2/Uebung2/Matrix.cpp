/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////

#include "Matrix.h"
#include "Vector.h"


CMat4d affineInverse(const CMat4d& M)
// Computes the inverse of a 4x4 matrix M of the form 
//             ⎡ R  P ⎤
//         M = ⎢      ⎥,
//             ⎣ 0  1 ⎦
// with an orthonormal 3x3 matrix R, a 3d-column vector P and a 3d-row vector 0=(0,0,0).
//
// The inverse if computed as
//				⎡ Rᵗ -Rᵗ·P ⎤
//      M⁻¹ =	⎢          ⎥ .
//				⎣  0   1   ⎦
//
// The steps in the computation are 
//			⎡ R  P ⎤		⎡ R  0 ⎤		⎡ Rᵗ  0 ⎤		⎡ Rᵗ -Rᵗ·P ⎤     ⎡ Rᵗ -Rᵗ·P ⎤
//		M = ⎢      ⎥	⇒¹	⎢      ⎥	⇒²	⎢       ⎥	⇒³	⎢          ⎥ ⇒⁴	⎢          ⎥ = M⁻¹ .
//			⎣ 0  1 ⎦		⎣ 0  1 ⎦		⎣ 0   1 ⎦		⎣  0   -1  ⎦     ⎣  0   1   ⎦
//
{
	CMat4d T = M;
	CVec4d P = M.getCol(3);	// step 0: copy P from M
	CVec4d b(0.0);
	b[3] = 1.0;

	T.setCol(b, 3);			// step ⇒¹: remove P from M		-> the upper left 3x3 block of M is R
	T.transpose();			// step ⇒²: transpose M			-> the upper left 3x3 block of M is Rᵗ 
	T.setCol(-(T * P), 3);	// step ⇒³: insert -Rᵗ·P into M	-> M⁻¹ with a negative entry M(3,3)
	T(3, 3) = 1.0;			// step ⇒⁴: correct M(3,3) to 1	-> M⁻¹

	return T;
}

