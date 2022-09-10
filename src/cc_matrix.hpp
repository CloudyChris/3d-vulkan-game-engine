// ProjectFilter(core)

#pragma once

#include "cm_commons.hpp"

template <class T>
struct CC_Matrix2_Generic
{
	// constructors
	CC_Matrix2_Generic()
	{
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				m[r][c] = 0;
			}
		}
	}

	CC_Matrix2_Generic(const CC_Matrix2_Generic& aStruct)
		: m(aStruct.m)
	{}

	CC_Matrix2_Generic(std::array<std::array<T,2>,2> _m)
		: m(_m)
	{}

	// members
	std::array<std::array<T, 2>, 2> m;

	// operator overloads
	CC_Matrix2_Generic& operator = (const CC_Matrix2_Generic& aMatrix) = default;

	CC_Matrix2_Generic operator + (const CC_Matrix2_Generic& aMatrix) const
	{
		CC_Matrix2_Generic matrix;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				matrix[r][c] = m[r][c] + aMatrix.m[r][c];
			}
		}

		return matrix;
	}

	CC_Matrix2_Generic operator - (const CC_Matrix2_Generic& aMatrix) const
	{
		CC_Matrix2_Generic matrix;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				matrix[r][c] = m[r][c] - aMatrix.m[r][c];
			}
		}

		return matrix;
	}

	CC_Matrix2_Generic operator * (const CC_Matrix2_Generic& aMatrix) const
	{
		CC_Matrix2_Generic matrix;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				matrix[r][c] = m[r][0] * aMatrix[0][c]
							 + m[r][1] * aMatrix[1][c];
			}
		}

		return matrix;
	}

	CC_Matrix2_Generic operator * (const T aScalar) const
	{
		CC_Matrix2_Generic<T> matrix;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				matrix[r][c] = m[r][c]*aScalar;
			}
		}

		return matrix;
	}

	CC_Matrix2_Generic operator / (const T aScalar) const
	{
		CC_Matrix2_Generic<T> matrix;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				matrix[r][c] = m[r][c]/aScalar;
			}
		}

		return matrix;
	}

	CC_Matrix2_Generic& operator *= (const T aScalar)
	{
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				this->m[r][c] *= aScalar;
			}
		}

		return this;
	}

	CC_Matrix2_Generic& operator /= (const T aScalar)
	{
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				this->m[r][c] /= aScalar;
			}
		}

		return this;
	}

	std::array<T, 2>& operator [] (const int aIndex)
	{
		return this->m[aIndex];
	}

	bool operator == (const CC_Matrix2_Generic& aMatrix)
	{
		bool flag = true;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				if (m[r][c] !=aMatrix[r][c])
				{
					flag = false;
				}
			}
		}

		return flag;
	}

	bool operator != (const CC_Matrix2_Generic& aMatrix)
	{
		bool flag = false;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				if (m[r][c] != aMatrix[r][c])
				{
					flag = true;
				}
			}
		}

		return flag;
	}

	// ostream support
	friend std::ostream& operator << (std::ostream& os, const CC_Matrix2_Generic& rhs)
	{
		os << rhs.ToString();
		return os;
	}

	// utilities
	const std::string ToString() const
	{
		std::string s;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				s += std::to_string(m[r][c]);
				if (r < 1)
				{
					s += ", ";
				}
			}
			s += "\n";
		}

		return s;
	}

	// matrix maths
	
	static CC_Matrix2_Generic Identity()
	{
		CC_Matrix2_Generic mI;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				mI[r][c] = (T)(c == r);
			}
		}

		return mI;
	}

	T Minor(uint r_current, uint c_current) const
	{
		uint _r = (r_current == 0) ? 1 : 0;
		uint _c = (c_current == 0) ? 1 : 0;
		return m[_r][_c];
	}

	T Det_Minor(uint r_current, uint c_current) const
	{
		return this->Minor(r_current, c_current);
	}

	CC_Matrix2_Generic Cofactor() const
	{
		CC_Matrix2_Generic C;

		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				C[r][c] = ExpSign(r+c)*this->Det_Minor(r, c);
			}
		}

		return C;
	}
	
	T Determinant() const
	{
		return m[0][0]*m[1][1] - m[0][1]*m[1][0];
	}

	CC_Matrix2_Generic Adjoint() const
	{
		return this->Cofactor().Transposed();
	}

	CC_Matrix2_Generic Transposed() const
	{
		std::array<std::array<T, 2>, 2> _m;
		for (uint c = 0; c < 2; c++)
		{
			for (uint r = 0; r < 2; r++)
			{
				_m[c][r] = m[r][c];
			}
		}

		return CC_Matrix2_Generic(_m);	
	}

	CC_Matrix2_Generic Inverse() const
	{
		T det = Determinant();
		CC_Matrix2_Generic adj = Adjoint();

		CC_Assert(det != 0, "Cannot compute inverse of matrix with determinant 0!");

		return adj*(1/det); 
	}
};


#define CC_Matrix2_i CC_Matrix2_Generic<int32_t>
#define CC_Matrix2_u CC_Matrix2_Generic<uint32_t>
#define CC_Matrix2_f CC_Matrix2_Generic<float>
#define CC_Matrix2_d CC_Matrix2_Generic<double>

template <class T>
struct CC_Matrix3_Generic
{
	// constructors
	CC_Matrix3_Generic()
	{
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				m[r][c] = 0;
			}
		}
	}

	CC_Matrix3_Generic(const CC_Matrix3_Generic& aStruct)
	{
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				m[r][c] = aStruct.m[r][c];
			}
		}

	}

	CC_Matrix3_Generic(std::array<std::array<T,3>,3> _m)
	{
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				m[r][c] = _m[r][c];
			}
		}
	}

	// members
	std::array<std::array<T, 3>, 3> m;

	// operator overloads
	CC_Matrix3_Generic& operator = (const CC_Matrix3_Generic& aMatrix) = default;

	CC_Matrix3_Generic operator + (const CC_Matrix3_Generic& aMatrix) const
	{
		CC_Matrix3_Generic matrix;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				matrix[r][c] = m[r][c] + aMatrix.m[r][c];
			}
		}

		return matrix;
	}

	CC_Matrix3_Generic operator - (const CC_Matrix3_Generic& aMatrix) const
	{
		CC_Matrix3_Generic matrix;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				matrix[r][c] = m[r][c] - aMatrix.m[r][c];
			}
		}

		return matrix;
	}

	CC_Matrix3_Generic operator * (const CC_Matrix3_Generic& aMatrix) const
	{
		CC_Matrix3_Generic matrix;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				matrix[r][c] = m[r][0] * aMatrix[0][c]
							 + m[r][1] * aMatrix[1][c]
							 + m[r][2] * aMatrix[2][c];
			}
		}

		return matrix;
	}

	CC_Matrix3_Generic operator * (const T aScalar) const
	{
		CC_Matrix3_Generic<T> matrix;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				matrix[r][c] = m[r][c]*aScalar;
			}
		}

		return matrix;
	}

	CC_Matrix3_Generic operator / (const T aScalar) const
	{
		CC_Matrix3_Generic<T> matrix;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				matrix[r][c] = m[r][c]/aScalar;
			}
		}

		return matrix;
	}

	CC_Matrix3_Generic& operator *= (const T aScalar)
	{
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				this->m[r][c] *= aScalar;
			}
		}
		return this;
	}

	CC_Matrix3_Generic& operator /= (const T aScalar)
	{
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				this->m[r][c] /= aScalar;
			}
		}

		return this;
	}

	std::array<T, 3>& operator [] (const int aIndex)
	{
		return this->m[aIndex];
	}

	bool operator == (const CC_Matrix3_Generic& aMatrix)
	{
		bool flag = true;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				if (m[r][c] !=aMatrix[r][c])
				{
					flag = false;
				}
			}
		}

		return flag;
	}

	bool operator != (const CC_Matrix3_Generic& aMatrix)
	{
		bool flag = false;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				if (m[r][c] != aMatrix[r][c])
				{
					flag = true;
				}
			}
		}

		return flag;
	}

	// ostream support
	friend std::ostream& operator << (std::ostream& os, const CC_Matrix3_Generic& rhs)
	{
		os << rhs.ToString();
		return os;
	}

	// utilities
	const std::string ToString() const
	{
		std::string s;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				s += std::to_string(m[r][c]);
				if (r < 2)
				{
					s += ", ";
				}
			}
			s += "\n";
		}

		return s;
	}

	// matrix maths
	
	static CC_Matrix3_Generic Identity()
	{
		CC_Matrix3_Generic mI;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				mI[r][c] = (T)(r == c);
			}
		}

		return mI;
	}

	CC_Matrix2_Generic<T> Minor(uint r_current, uint c_current) const
	{
		CC_Matrix2_Generic<T> M;
		uint _c = 0;
		uint _r = 0;
		for (uint c = 0; c < 3; c++)
		{
			if (c == c_current)
			{
				continue;
			}

			for (uint r = 0; r < 3; r++)
			{
				if (r == r_current)
				{
					continue;
				}

				M[_r][_c] = m[r][c];
				_c++;

				if (_c == 2)
				{
					_c = 0;
					_r++;
				}
			}
		}

		return M;
	}
	T Det_Minor(uint r_current, uint c_current) const
	{
		return Minor(r_current, c_current).Determinant();
	}

	CC_Matrix3_Generic Cofactor() const
	{
		CC_Matrix3_Generic C;

		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				C[r][c] = ExpSign(r+c)*this->Det_Minor(r, c);
			}
		}

		return C;
	}

	T Determinant() const
	{
		T sum = 0;
		for (uint c = 0; c < 3; c++)
		{
			sum += ExpSign(c)*m[0][c]*Det_Minor(0, c);
		}

		return sum;
	}

	CC_Matrix3_Generic Adjoint() const
	{
		return Cofactor().Transposed();
	}

	CC_Matrix3_Generic Transposed() const
	{
		std::array<std::array<T, 3>, 3> _m;
		for (uint c = 0; c < 3; c++)
		{
			for (uint r = 0; r < 3; r++)
			{
				_m[c][r] = m[r][c];
			}
		}

		return CC_Matrix3_Generic(_m);	
	}

	CC_Matrix3_Generic Inverse() const
	{
		T det = Determinant();
		CC_Matrix3_Generic adj = Adjoint();
		CC_Assert(det != 0, "Cannot compute inverse of matrix with determinant 0!");

		return adj*(1/det);
	}
};


#define CC_Matrix3_i CC_Matrix3_Generic<int32_t>
#define CC_Matrix3_u CC_Matrix3_Generic<uint32_t>
#define CC_Matrix3_f CC_Matrix3_Generic<float>
#define CC_Matrix3_d CC_Matrix3_Generic<double>

template <class T>
struct CC_Matrix4_Generic
{
	// constructors
	CC_Matrix4_Generic()
	{
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				m[r][c] = 0;
			}
		}
	}

	CC_Matrix4_Generic(const CC_Matrix4_Generic& aStruct)
	{
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				m[r][c] = aStruct.m[r][c];
			}
		}

	}

	CC_Matrix4_Generic(std::array<std::array<T,4>,4> _m)
	{
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				m[r][c] = _m[r][c];
			}
		}
	}

	// members
	std::array<std::array<T, 4>, 4> m;

	// operator overloads
	CC_Matrix4_Generic& operator = (const CC_Matrix4_Generic& aMatrix) = default;

	CC_Matrix4_Generic operator + (const CC_Matrix4_Generic& aMatrix) const
	{
		CC_Matrix4_Generic matrix;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				matrix[r][c] = m[r][c] + aMatrix.m[r][c];
			}
		}

		return matrix;
	}

	CC_Matrix4_Generic operator - (const CC_Matrix4_Generic& aMatrix) const
	{
		CC_Matrix4_Generic matrix;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				matrix[r][c] = m[r][c] - aMatrix.m[r][c];
			}
		}

		return matrix;
	}

	CC_Matrix4_Generic operator * (const CC_Matrix4_Generic& aMatrix) const
	{
		CC_Matrix4_Generic matrix;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				matrix[r][c] = m[r][0] * aMatrix[0][c]
							 + m[r][1] * aMatrix[1][c]
							 + m[r][2] * aMatrix[2][c]
							 + m[r][3] * aMatrix[3][c];
			}
		}

		return matrix;
	}

	CC_Matrix4_Generic operator * (const T aScalar) const
	{
		CC_Matrix4_Generic<T> matrix;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				matrix[r][c] = m[r][c]*aScalar;
			}
		}

		return matrix;
	}

	CC_Matrix4_Generic operator / (const T aScalar) const
	{
		CC_Matrix4_Generic<T> matrix;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				matrix[r][c] = m[r][c]/aScalar;
			}
		}

		return matrix;
	}

	CC_Matrix4_Generic& operator *= (const T aScalar)
	{
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				this->m[r][c] *= aScalar;
			}
		}
		return this;
	}

	CC_Matrix4_Generic& operator /= (const T aScalar)
	{
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				this->m[r][c] /= aScalar;
			}
		}

		return this;
	}

	std::array<T, 4>& operator [] (const int aIndex)
	{
		return this->m[aIndex];
	}

	bool operator == (const CC_Matrix4_Generic& aMatrix)
	{
		bool flag = true;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				if (m[r][c] !=aMatrix[r][c])
				{
					flag = false;
				}
			}
		}

		return flag;
	}

	bool operator != (const CC_Matrix4_Generic& aMatrix)
	{
		bool flag = false;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				if (m[r][c] != aMatrix[r][c])
				{
					flag = true;
				}
			}
		}

		return flag;
	}

	// ostream support
	friend std::ostream& operator << (std::ostream& os, const CC_Matrix4_Generic& rhs)
	{
		os << rhs.ToString();
		return os;
	}

	// utilities
	const std::string ToString() const
	{
		std::string s;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				s += std::to_string(m[r][c]);
				if (r < 3)
				{
					s += ", ";
				}
			}
			s += "\n";
		}

		return s;
	}

	// matrix maths
	
	static CC_Matrix4_Generic Identity()
	{
		CC_Matrix4_Generic mI;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				mI[r][c] = (T)(r == c);
			}
		}

		return mI;
	}

	CC_Matrix3_Generic<T> Minor(uint r_current, uint c_current) const
	{
		CC_Matrix3_Generic<T> M;
		uint _c = 0;
		uint _r = 0;
		for (uint c = 0; c < 4; c++)
		{
			if (c == c_current)
			{
				continue;
			}

			for (uint r = 0; r < 4; r++)
			{
				if (r == r_current)
				{
					continue;
				}

				M[_r][_c] = m[r][c];
				_c++;

				if (_c == 3)
				{
					_c = 0;
					_r++;
				}
			}
		}

		return M;
	}
	T Det_Minor(uint r_current, uint c_current) const
	{
		return Minor(r_current, c_current).Determinant();
	}

	CC_Matrix4_Generic Cofactor() const
	{
		CC_Matrix4_Generic C;

		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				C[r][c] = ExpSign(r+c)*this->Det_Minor(r, c);
			}
		}

		return C;
	}

	T Determinant() const
	{
		T sum = 0;
		for (uint c = 0; c < 4; c++)
		{
			sum += ExpSign(c)*m[0][c]*Det_Minor(0, c);
		}

		return sum;
	}

	CC_Matrix4_Generic Adjoint() const
	{
		return Cofactor().Transposed();
	}

	CC_Matrix4_Generic Transposed() const
	{
		std::array<std::array<T, 4>, 4> _m;
		for (uint c = 0; c < 4; c++)
		{
			for (uint r = 0; r < 4; r++)
			{
				_m[c][r] = m[r][c];
			}
		}

		return CC_Matrix4_Generic(_m);	
	}

	CC_Matrix4_Generic Inverse() const
	{
		T det = Determinant();
		CC_Matrix4_Generic adj = Adjoint();
		CC_Assert(det != 0, "Cannot compute inverse of matrix with determinant 0!");

		return adj*(1/det);
	}
};


#define CC_Matrix4_i CC_Matrix4_Generic<int32_t>
#define CC_Matrix4_u CC_Matrix4_Generic<uint32_t>
#define CC_Matrix4_f CC_Matrix4_Generic<float>
#define CC_Matrix4_d CC_Matrix4_Generic<double>

