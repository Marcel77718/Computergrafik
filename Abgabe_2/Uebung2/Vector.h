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

#include <iostream>

template<class T, unsigned n> class CMatrix;		// forward declaration of n x n matrix

//
// generic n-dimensional vector class for SIMPLE data types T
//
template <class T, unsigned n> class CVector {
private:
	T m_atData[n];		// array of size n of type T

public:
	// standard/default constructor
	CVector<T,n> (T x=T(0), T y = T(0), T z = T(0))
	{
		unsigned m = (n < 3 ? n : 3);
		T tmp[3] = { x, y, z };
		for (unsigned i = 0; i < m; i++) m_atData[i] = tmp[i];
		for (unsigned i = m; i < n; i++) m_atData[i] = T(0);
	}

	// constructor from data array
	CVector<T, n> (const T atData[n])							
	{
		for (unsigned i=0; i<n; i++) m_atData[i] = atData[i];
	}

	// copy constructor
	CVector<T, n> (const CVector<T, n> &vec)					
	{
		for (unsigned i=0; i<n; i++) m_atData[i] = vec.m_atData[i];
	}

	// constructor from n-1 to n dimensions with new element t
	CVector<T, n> (const CVector<T, n-1> &vec, T t)				
	{
		for (unsigned i=0; i<n-1; i++) m_atData[i] = vec[i];
		m_atData[n-1] = t;
	}

	// constructor from n+1 to n dimensions 
	CVector<T, n> (const CVector<T, n+1> &vec) 					
	{
		for (unsigned i=0; i<n; i++) m_atData[i] = vec[i];
	}

	// destructor
	~CVector () {/* nothing to do here ...*/}					

	// setter
	void setData (const T atData[n]) 
	{
		for (unsigned i=0; i<n; i++) m_atData[i] = atData[i];
	}

	// getter
	void getData (T atData[n]) const
	{
		for (unsigned i=0; i<n; i++) atData[i] = m_atData[i];
	}

	unsigned getDimension () 
	{
		return n;
	}

	// copy assignment operator
	CVector<T, n> &operator = (const CVector<T, n> &vec)		
	{
		if (this==&vec)	return (*this);							// ok, it's me, so no L-value action
		for (int i=0; i<n; i++)	m_atData[i] = vec.m_atData[i];	// not me, so L-Value action: copy data
		return (*this); 
	}

	// assignment from data array
	CVector<T, n> &operator = (const T atData[n])				
	{
		for (unsigned i=0; i<n; i++) m_atData[i] = atData[i];	// copy data
		return (*this); 
	}

	// index operator
	T& operator [] (unsigned i)
	{	
		return m_atData[(i+n)%n];	
	}

	// const index operator
	T operator [] (unsigned i) const 
	{
		return m_atData[(i + n) % n];	
	}

	// unitary minus: flip orientation
	CVector<T, n> operator - () const
	{
		T atBuffer[n];
		for (unsigned i = 0; i < n; i++) atBuffer[i] = -m_atData[i];
		return CVector<T, n>(atBuffer);
	}

	// vector addition
	void operator += (const CVector<T, n> &vec)				
	{
		for (unsigned i=0; i<n; i++) m_atData[i] += vec.m_atData[i];
	}

	// vector addition
	CVector<T, n> operator + (const CVector<T, n> &vec) const		
	{
		CVector<T, n> buf (m_atData);
		for (unsigned i=0; i<n; i++) buf.m_atData[i] += vec.m_atData[i];
		return buf;
	}

	// vector subtraction 
	void operator -= (const CVector<T, n> &vec)				
	{
		for (unsigned i=0; i<n; i++) m_atData[i] -= vec.m_atData[i];
	}
	
	// vector subtraction 
	CVector<T, n> operator - (const CVector<T, n> &vec) const
	{
		CVector<T, n> buf (m_atData);
		for (unsigned i=0; i<n; i++) buf.m_atData[i] -= vec.m_atData[i];
		return buf;
	}

	// vector-scalar multiplication
	void operator *= (T tScale)
	{
		for (unsigned i = 0; i < n; i++) m_atData[i] *= tScale;
	}

	// vector-scalar-multiplication
	CVector<T, n> operator * (T tScale) const
	{
		T atBuffer[n];
		for (unsigned i = 0; i < n; i++) atBuffer[i] = m_atData[i] * tScale;
		return CVector<T, n>(atBuffer);
	}

	// vector-scalar division
	void operator /= (T tScale)
	{
		for (unsigned i = 0; i < n; i++) m_atData[i] /= tScale;
	}

	// vector-vector-multiplication, aka scalar-product
	T operator * (const CVector<T, n>& vec) const
	{
		T dp = T(0);
		for (unsigned i = 0; i < n; i++) dp += m_atData[i] * vec.m_atData[i];
		return dp;
	}

	// vector-vector-multiplication, aka scalar-product
	T dot(const CVector<T, n>& vec) const
	{
		return (*this) * vec;
	}

	// cross-product: defined for CVec4. Fourth coordinates is ignored and set to 0.
	CVector<T, 4> crossH (const CVector<T, 4> &vec) const
	{
		T atBuffer[4];
		atBuffer[0] = m_atData[1] * vec.m_atData[2];
		atBuffer[0]-= m_atData[2] * vec.m_atData[1];
		atBuffer[1] = m_atData[2] * vec.m_atData[0];
		atBuffer[1]-= m_atData[0] * vec.m_atData[2];
		atBuffer[2] = m_atData[0] * vec.m_atData[1];
		atBuffer[2]-= m_atData[1] * vec.m_atData[0];
		atBuffer[3] = 0;
		return CVector<T, 4> (atBuffer);
	}
	
	// cross-product: defined for CVec3.
	CVector<T, 3> cross (const CVector<T, 3> &vec) const
	{
		T atBuffer[3];
		atBuffer[0] = m_atData[1] * vec.m_atData[2];
		atBuffer[0]-= m_atData[2] * vec.m_atData[1];
		atBuffer[1] = m_atData[2] * vec.m_atData[0];
		atBuffer[1]-= m_atData[0] * vec.m_atData[2];
		atBuffer[2] = m_atData[0] * vec.m_atData[1];
		atBuffer[2]-= m_atData[1] * vec.m_atData[0];
		return CVector<T, 3> (atBuffer);
	}

	// norm of a vector
	T norm() const
	{
		T d = 0;
		for (unsigned i = 0; i < n; i++) d += m_atData[i] * m_atData[i];
		return sqrt(d);
	}

	// length of a vector
	T length() const
	{
		return norm();
	}

	// normalizes a vector to norm/length 1
	 void normalize ()
	{
		T d = norm();
		if (d == 0.0) return;
		(*this) *= 1.0 / d;
	}	
	
	// normalizes a vector to norm/length 1
	CVector<T,n> normalized() const
	{
		CVector<T, n> d(* this);
		d.normalize();
		return d;
	}

	// homogenizes a vector by division by the last entry
	void homogenize()
	{
		T d = m_atData[n-1];
		if (d == 0.0) return;
		(*this) *= 1.0 / d;
	}
}; // class CVector


template <class T, unsigned n> std::ostream& operator << (std::ostream& os, const CVector<T, n>& c)
{
	os << "(";
	for (unsigned j = 0; j < n - 1; j++) os << c[j] << ",";
	os << c[n - 1] << ")";
	return os;
}

// some common vector classes (abbr. names)
typedef CVector<float, 2> CVec2f;
typedef CVector<float, 3> CVec3f;
typedef CVector<float, 4> CVec4f;

typedef CVector<double, 2> CVec2d;
typedef CVector<double, 3> CVec3d;
typedef CVector<double, 4> CVec4d;

typedef CVector<int, 2> CVec2i;
typedef CVector<int, 3> CVec3i;
typedef CVector<int, 4> CVec4i;

typedef CVector<bool, 2> CVec2b;
typedef CVector<bool, 3> CVec3b;
typedef CVector<bool, 4> CVec4b;