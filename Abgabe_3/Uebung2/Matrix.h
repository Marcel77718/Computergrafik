/////////////////////////////////////////////////////////////////////////////
//																									//
// Code framework for lecture computer graphics WS 2025/26 assignment 2+3	//
//																									//
// A generic class for n-dimensional coordinate vectors							//
//																									//
// (c) Georg Umlauf																			//
//																									//
/////////////////////////////////////////////////////////////////////////////


#pragma once

template <class T, unsigned n> class CVector;				// forward declaration of n-dimensional vector

//
//
// generic n x n matrix class for SIMPLE data types T
//
//
template <class T, unsigned n> class CMatrix {
private:
	T m_aatData[n][n] = {T(0)};		// array of size n x n of type T

public:
	// default/standard constructor
	CMatrix<T, n> (T t=T(0)) {								
		for (unsigned j=0; j<n; j++)
			for (unsigned i=0; i<n; i++)
				m_aatData[i][j] = t;// initialize all elements with t or zero						
	}

	// constructor from array
	CMatrix<T, n> (const T aatData[n][n]) {					
		for (unsigned j=0; j<n; j++) 
			for (unsigned i=0; i<n; i++) 
				m_aatData[i][j] = aatData[i][j];
	}

	// copy constructor
	CMatrix<T, n> (const CMatrix<T, n> &mat) {				
		for (unsigned j=0; j<n; j++)
			for (unsigned i=0; i<n; i++)
				m_aatData[i][j] = mat.m_aatData[i][j];
	}

	static CMatrix<T, n> Identity() {
		CMatrix<T, n> tmp;
		for (unsigned i = 0; i < n; i++) tmp(i, i) = 1.0;
		return tmp;
	}

	// destructor
	~CMatrix<T, n>() {/* nothing to do here ...*/ }	

	// index operator
	T &operator () (unsigned i, unsigned j) {
		return m_aatData[(i + n) % n][(j + n) % n];
	}

	// index operator
	T operator () (unsigned i, unsigned j) const {
		return m_aatData[(i + n) % n][(j + n) % n];
	}

	// matrix-matrix-multiplication
	CMatrix<T, n> operator * (const CMatrix<T, n> &mat) const {
		CMatrix<T, n> buf(T(0));					// zero matrix
		for (unsigned i=0; i<n; i++)				// row i
			for (unsigned j=0; j<n; j++)			// column j
				for (unsigned k=0; k<n; k++)		// k
					buf(i,j) += m_aatData[i][k] * mat(k,j);
		return buf;
	}

	// matrix-vector-multiplication
	CVector<T, n> operator * (const CVector<T, n> &vec) const {	
		CVector<T, n> buf(T(0));					// zero vector
		for (unsigned j=0; j<n; j++)				// column j
			for (unsigned i=0; i<n; i++)			// row i
				buf[i] += m_aatData[i][j]*vec[j];
		return buf;
	}

	CVector<T, n> getCol(int j)  const {
		CVector<T, n> buf;							// zero vector
		for (unsigned i = 0; i < n; i++)			// row i
			buf[i] = m_aatData[i][(j + n) % n];
		return buf;
	}

	void setCol(const CVector<T, n>& buf, int j) {
		for (unsigned i = 0; i < n; i++)			// row i
			m_aatData[i][(j + n) % n] = buf[i];
	}

	CVector<T, n> getRow(int i) const {
		CVector<T, n> buf;							// zero vector
		for (unsigned j = 0; j < n; j++)			// column j
			buf[j] = m_aatData[(i + n) % n][j];
		return buf;
	}

	void setRow(const CVector<T, n>& buf, int i) {
		for (unsigned j = 0; j < n; j++)			// column j
			m_aatData[(i + n) % n][j] = buf[j];
	}


	void transpose() {
		for (unsigned i = 0; i < n; i++)
			for (unsigned j = 0; j < i; j++)
				swap(i, j);
	}

	void swap(int i, int j) {
		int I = (i + n) % n, J = (j + n) % n;
		T x = m_aatData[I][J];
		m_aatData[I][J] = m_aatData[J][I];
		m_aatData[J][I] = x;
	}

	// CMatrix<T, SIZE>operator + (const Matrix<T, SIZE> &mat)	// implement yourself, if required
	// CMatrix<T, SIZE>operator - (const Matrix<T, SIZE> &mat)	// implement yourself, if required
};

// some common vector classes (abbr. names)
typedef CMatrix<float, 2> CMat2f;
typedef CMatrix<float, 3> CMat3f;
typedef CMatrix<float, 4> CMat4f;

typedef CMatrix<double, 2> CMat2d;
typedef CMatrix<double, 3> CMat3d;
typedef CMatrix<double, 4> CMat4d;

CMat4d affineInverse(const CMat4d& M);	// inverse of AFFINE MAP only!!!
