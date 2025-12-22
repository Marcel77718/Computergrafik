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

CMat4f getTransform2(CVec4f ViewOrigin, CVec4f ViewDir, CVec4f ViewUp)
{
	//translation into world origin
	CMat4f MT;
	MT = MT.Identity();
	MT.setCol(-ViewOrigin, 3);

	//angle between ViewDir and y-(-z)-plane
	CVec2f viewInXZ;
	viewInXZ[0] = ViewDir[0];//x
	viewInXZ[1] = -ViewDir[2];//-z
	viewInXZ.normalize();
	float cos_phi = viewInXZ.dot(CVec2f(0.0, 1.0));
	float sin_phi = viewInXZ.dot(CVec2f(1.0, 0.0));
	std::cout << "ViewDir: " << ViewDir[0] << ViewDir[1] << ViewDir[2] << ViewDir[3] << "\n";
	std::cout << "viewInXZ: " << viewInXZ << "\n";
	std::cout << "phi: " << atan2(sin_phi, cos_phi) << "\n";
	std::cout << "cos: " << cos_phi << ", sin: " << sin_phi << std::endl;

	//Rotation around world y into y-(-z)-plane
	CMat4f MR;
	MR = MR.Identity();
	MR(0, 0) = cos_phi; MR(0, 2) = sin_phi;
	MR(2, 0) = -sin_phi; MR(2, 2) = cos_phi;

	//angle between ViewDir and x-(-z)-plane
	CVec4f dirYawed = MR * ViewDir;
	CVec2f viewInYZ(dirYawed[1], -dirYawed[2]);
	viewInYZ.normalize();
	float cos_theta = viewInYZ.dot(CVec2f(0.0, 1.0));
	float sin_theta = viewInYZ.dot(CVec2f(1.0, 0.0)); //beide nochmal kontrollieren!
	std::cout << "cos: " << cos_theta << ", sin: " << sin_theta << std::endl;

	//Rotation around world x from y-z-plane into z-axis
	CMat4f MR2;
	MR2 = MR2.Identity();
	MR2(1, 1) = cos_theta; MR2(1, 2) = -sin_theta;
	MR2(2, 1) = sin_theta; MR2(2, 2) = cos_theta;

	//angle between rotated ViewUp and world y
	CVec4f rotatedUp = MR2 * MR * ViewUp;
	CVec2f rotatedUp2 = CVec2f(rotatedUp[0], rotatedUp[1]);
	float cos_gamma = rotatedUp2.dot(CVec2f(0.0, 1.0));
	float sin_gamma = rotatedUp2.dot(CVec2f(1.0, 0.0));
	std::cout << "rotatedUp: " << rotatedUp << "\n";
	std::cout << "cos: " << cos_gamma << ", sin: " << sin_gamma << std::endl;

	//Rotation around z-axis to align both ys
	CMat4f MR3;
	MR3 = MR3.Identity();
	MR3(0, 0) = cos_gamma; MR3(0, 1) = -sin_gamma;
	MR3(1, 0) = sin_gamma; MR3(1, 1) = cos_gamma;

	return MR3 * MR2 * MR * MT;
}

CMat4f getTransform3(CVec4f ViewOrigin, CVec4f ViewDir, CVec4f ViewUp)
{
	CVec3f f = CVec3f(-ViewDir[0], -ViewDir[1], -ViewDir[2]);
	f.normalize();
	CVec3f r = CVec3f(ViewUp[0], ViewUp[1], ViewUp[2]).cross(f);
	r.normalize();
	CVec3f u = f.cross(r);

	CMat4f M = CMat4f::Identity();

	M(0, 0) = r[0]; M(1, 0) = r[1]; M(2, 0) = r[2];
	M(0, 1) = u[0]; M(1, 1) = u[1]; M(2, 1) = u[2];
	M(0, 2) = f[0]; M(1, 2) = f[1]; M(2, 2) = f[2];

	CVec3f ViewO = CVec3f(ViewOrigin[0], ViewOrigin[1], ViewOrigin[2]);
	M(3, 0) = -r.dot(ViewO);
	M(3, 1) = -u.dot(ViewO);
	M(3, 2) = -f.dot(ViewO);

	return M;
}


