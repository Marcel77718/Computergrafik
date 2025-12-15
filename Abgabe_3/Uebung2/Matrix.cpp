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

CMat4f getTransform(CVec4f ViewOrigin, CVec4f ViewDir, CVec4f ViewUp)
{
	// Create the transformation matrix from world coordinates to view coordinates
	CVec4f zvv = -ViewDir; // zzvv-axis is anti-parallel to ViewDir
	CVec4f yvv = ViewUp;   // yyvv-axis is parallel to ViewUp
	float crossprod[4] = { yvv[1] * zvv[2] - yvv[2] * zvv[1],
		zvv[0] * yvv[2] - zvv[2] * yvv[0],
		yvv[0] * zvv[1] - yvv[1] * zvv[0],
		0.0f };
	CVec4f xvv = CVec4f(crossprod); // xxvv-axis is cross product of yyvv and zzvv
	// normalize the axes
	xvv = xvv * (1.0f / sqrtf(xvv[0] * xvv[0] + xvv[1] * xvv[1] + xvv[2] * xvv[2]));
	yvv = yvv * (1.0f / sqrtf(yvv[0] * yvv[0] + yvv[1] * yvv[1] + yvv[2] * yvv[2]));
	zvv = zvv * (1.0f / sqrtf(zvv[0] * zvv[0] + zvv[1] * zvv[1] + zvv[2] * zvv[2]));
	CMat4f M; // transformation matrix
	M(0, 0) = xvv[0]; M(0, 1) = yvv[0]; M(0, 2) = zvv[0]; M(0, 3) = ViewOrigin[0];
	M(1, 0) = xvv[1]; M(1, 1) = yvv[1]; M(1, 2) = zvv[1]; M(1, 3) = ViewOrigin[1];
	M(2, 0) = xvv[2]; M(2, 1) = yvv[2]; M(2, 2) = zvv[2]; M(2, 3) = ViewOrigin[2];
	M(3, 0) = 0.0f;   M(3, 1) = 0.0f;   M(3, 2) = 0.0f;   M(3, 3) = 1.0f;

	double Mat_array[4][4]; 
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Mat_array[i][j] = static_cast<double>(M(i, j));
		}
	}
	CMat4d MatrixDouble = affineInverse(CMat4d(Mat_array));
	float Mat_array_f[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Mat_array_f[i][j] = static_cast<float>(MatrixDouble(i, j));
		}
	}
	return CMat4f(Mat_array_f);

}

