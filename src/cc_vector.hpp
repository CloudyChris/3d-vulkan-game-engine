// ProjectFilter(core)

#pragma once

#include "cm_commons.hpp"

template <class T>
struct CC_Vector2_Generic
{
public:
	// constructors
	CC_Vector2_Generic()
		: x(0)
		, y(0)
	{}

	CC_Vector2_Generic(T _x, T _y)
		: x(_x)	
		, y(_y)
	{}

	CC_Vector2_Generic(const CC_Vector2_Generic& aVector)
		: x(aVector.x)
		, y(aVector.y)
	{}

	// members
	T x;
	T y;

	// operator overloads
	CC_Vector2_Generic& operator = (const CC_Vector2_Generic& aVector) = default;

	T operator [] (const uint aIndex) const
	{
		CC_Assert(aIndex < sizeof(CC_Vector2_Generic<T>)/sizeof(T), "Index is out of bounds");
		return *(T*)((T*)this + aIndex);
	}

	CC_Vector2_Generic operator + (const CC_Vector2_Generic& aVector) const
	{
		return CC_Vector2_Generic(x + aVector.x, y + aVector.y);
	}

	CC_Vector2_Generic operator - (const CC_Vector2_Generic& aVector) const
	{
		return CC_Vector2_Generic(x - aVector.x, y - aVector.y);
	}

	CC_Vector2_Generic operator * (const CC_Vector2_Generic& aVector) const
	{
		return CC_Vector2_Generic(x * aVector.x, y * aVector.y);
	}

	CC_Vector2_Generic operator * (const T rhs) const
	{
		return CC_Vector2_Generic(x * rhs, y * rhs);
	}

	CC_Vector2_Generic operator / (const CC_Vector2_Generic& aVector) const
	{
		return CC_Vector2_Generic(x / aVector.x, y / aVector.y);
	}

	CC_Vector2_Generic operator / (const T rhs) const
	{
		return CC_Vector2_Generic(x / rhs, y / rhs);
	}

	CC_Vector2_Generic operator += (const CC_Vector2_Generic& aVector) const
	{
		this->x += aVector.x;
		this->y += aVector.y;
		return *this;
	}

	CC_Vector2_Generic operator -= (const CC_Vector2_Generic& aVector) const
	{
		this->x -= aVector.x;
		this->y -= aVector.y;
		return *this;
	}

	CC_Vector2_Generic operator *= (const CC_Vector2_Generic& aVector) const
	{
		this->x *= aVector.x;
		this->y *= aVector.y;
		return *this;
	}

	CC_Vector2_Generic operator *= (const T rhs) const
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	CC_Vector2_Generic operator /= (const CC_Vector2_Generic& aVector) const
	{
		this->x /= aVector.x;
		this->y /= aVector.y;
		return *this;
	}

	CC_Vector2_Generic operator /= (const T rhs) const
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}

	CC_Vector2_Generic operator + () const
	{
		return CC_Vector2_Generic(+x, +y);
	}

	CC_Vector2_Generic operator - () const
	{
		return CC_Vector2_Generic(-x, -y);
	}

	bool operator == (const CC_Vector2_Generic& aVector) const
	{
		return (x == aVector.x && y == aVector.y);
	}

	bool operator != (const CC_Vector2_Generic& aVector) const
	{
		return (x != aVector.x || y != aVector.y);
	}

	// std::ostream support
	friend std::ostream& operator << (std::ostream& os, const CC_Vector2_Generic& rhs)
	{
		os << rhs.ToString();
		return os;
	}

	// utilities
	const std::string ToString() const
	{
		std::string s;
		s += "[";	
		for (uint i = 0; i < 2; i++)
		{
			s += std::to_string((*this)[i]);
			if (i < 1)
			{
				s += ", ";
			}
		}
		s += "]";
		return s;
	}

	// vector maths 
	T Magnitude () const
	{
		return T(sqrtf(x*x + y*y));
	}

	T Magnitude2 () const
	{
		return (x*x + y*y);
	}

	CC_Vector2_Generic Normalized () const
	{
		T r = 1 / Magnitude2();
		return CC_Vector2_Generic(x*r, y*r);
	}

	CC_Vector2_Generic Perpendicular () const
	{
		return CC_Vector2_Generic(-y, x);
	}

	T Dot (const CC_Vector2_Generic& aVector) const
	{
		return (x*aVector.x + y*aVector.y);
	}

	T Cross (const CC_Vector2_Generic& aVector) const
	{
		return (x*aVector.y - y*aVector.x);
	}

	// vector maths extension
	
	CC_Vector2_Generic Floor () const
	{
		return CC_Vector2_Generic(std::floor(x), std::floor(y)); 
	}

	CC_Vector2_Generic Ceil () const
	{
		return CC_Vector2_Generic(std::ceil(x), std::ceil(y)); 
	}

	CC_Vector2_Generic Max (const CC_Vector2_Generic& aVector)
	{
		return CC_Vector2_Generic(std::max(x, aVector.x), std::max(y, aVector.y));
	}

	CC_Vector2_Generic Min (const CC_Vector2_Generic& aVector)
	{
		return CC_Vector2_Generic(std::min(x, aVector.x), std::min(y, aVector.y));
	}

	CC_Vector2_Generic Catersian ()
	{
		return CC_Vector2_Generic(std::cos(y) * x, std::sin(y) * x);
	}

	CC_Vector2_Generic Polar ()
	{
		return CC_Vector2_Generic(Magnitude2(), std::atan2(y, x));
	}

};

template<class T, class U>
inline bool operator < (const CC_Vector2_Generic<T>& lhs, const CC_Vector2_Generic<U>& rhs)
{
	return (lhs.Magnitude() < rhs.Magnitude());
}

template<class T, class U>
inline bool operator > (const CC_Vector2_Generic<T>& lhs, const CC_Vector2_Generic<U>& rhs)
{
	return (lhs.Magnitude() > rhs.Magnitude());
}

#define CC_Vector2d_i CC_Vector2_Generic<int32_t>
#define CC_Vector2d_u CC_Vector2_Generic<uint32_t>
#define CC_Vector2d_f CC_Vector2_Generic<float>
#define CC_Vector2d_d CC_Vector2_Generic<double>

template<class T>
struct CC_Vector3_Generic
{
public:
	// constructors
	CC_Vector3_Generic()
		: x(0)
		, y(0)
		, z(0)
	{}

	CC_Vector3_Generic(T _x, T _y, T _z)
		: x(_x)	
		, y(_y)
		, z(_z)
	{}

	CC_Vector3_Generic(const CC_Vector3_Generic& aVector)
		: x(aVector.x)
		, y(aVector.y)
		, z(aVector.z)
	{}

	// members
	T x;
	T y;
	T z;

	// operator overloads
	CC_Vector3_Generic& operator = (const CC_Vector3_Generic& aVector) = default;

	T operator [] (const uint aIndex) const
	{
		CC_Assert(aIndex < sizeof(CC_Vector3_Generic<T>)/sizeof(T), "Index is out of bounds");
		return *(T*)((T*)this + aIndex);
	}

	CC_Vector3_Generic operator + (const CC_Vector3_Generic& aVector) const
	{
		return CC_Vector3_Generic(x + aVector.x, y + aVector.y, z + aVector.z);
	}

	CC_Vector3_Generic operator - (const CC_Vector3_Generic& aVector) const
	{
		return CC_Vector3_Generic(x - aVector.x, y - aVector.y, z - aVector.z);
	}

	CC_Vector3_Generic operator * (const CC_Vector3_Generic& aVector) const
	{
		return CC_Vector3_Generic(x * aVector.x, y * aVector.y, z * aVector.z);
	}

	CC_Vector3_Generic operator * (const T rhs) const
	{
		return CC_Vector3_Generic(x * rhs, y * rhs, z * rhs);
	}

	CC_Vector3_Generic operator / (const CC_Vector3_Generic& aVector) const
	{
		return CC_Vector3_Generic(x / aVector.x, y / aVector.y, z / aVector.z);
	}

	CC_Vector3_Generic operator / (const T rhs) const
	{
		return CC_Vector3_Generic(x / rhs, y / rhs, z / rhs);
	}

	CC_Vector3_Generic operator += (const CC_Vector3_Generic& aVector) const
	{
		this->x += aVector.x;
		this->y += aVector.y;
		this->z += aVector.z;
		return *this;
	}

	CC_Vector3_Generic operator -= (const CC_Vector3_Generic& aVector) const
	{
		this->x -= aVector.x;
		this->y -= aVector.y;
		this->z -= aVector.z;
		return *this;
	}

	CC_Vector3_Generic operator *= (const CC_Vector3_Generic& aVector) const
	{
		this->x *= aVector.x;
		this->y *= aVector.y;
		this->z *= aVector.z;
		return *this;
	}

	CC_Vector3_Generic operator *= (const T rhs) const
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		return *this;
	}

	CC_Vector3_Generic operator /= (const CC_Vector3_Generic& aVector) const
	{
		this->x /= aVector.x;
		this->y /= aVector.y;
		this->z /= aVector.z;
		return *this;
	}

	CC_Vector3_Generic operator /= (const T rhs) const
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		return *this;
	}

	CC_Vector3_Generic operator + () const
	{
		return CC_Vector3_Generic(+x, +y, +z);
	}

	CC_Vector3_Generic operator - () const
	{
		return CC_Vector3_Generic(-x, -y, -z);
	}

	bool operator == (const CC_Vector3_Generic& aVector) const
	{
		return (x == aVector.x && y == aVector.y && z == aVector.z);
	}

	bool operator != (const CC_Vector3_Generic& aVector) const
	{
		return (x != aVector.x || y != aVector.y || z != aVector.z);
	}

	// std::ostream support
	friend std::ostream& operator << (std::ostream& os, const CC_Vector3_Generic& rhs)
	{
		os << rhs.ToString();
		return os;
	}

	// utilities
	const std::string ToString() const
	{
			std::string s;
		s += "[";
		for (uint i = 0; i < 3; i++)
		{
			s += std::to_string(this[i]);
			s += ", ";
		}
		s += "]";
		return s;
	}

	// vector maths methods
	T Magnitude () const
	{
		return T(sqrtf(x*x + y*y + z*z));
	}

	T Magnitude2 () const
	{
		return (x*x + y*y + z*z);
	}

	CC_Vector3_Generic Normalized () const
	{
		T r = 1 / Magnitude2();
		return CC_Vector3_Generic(x*r, y*r, z*r);
	}

	T Dot (const CC_Vector3_Generic& aVector) const
	{
		return (x*aVector.x + y*aVector.y + z*aVector.z);
	}

	CC_Vector3_Generic Cross (const CC_Vector3_Generic& aVector) const
	{
		return CC_Vector3_Generic(y*aVector.z - z*aVector.y, x*aVector.z - z*aVector.x, x*aVector.y - y*aVector.x);
	}

	// vector maths extension
	
	CC_Vector3_Generic Floor () const
	{
		return CC_Vector3_Generic(std::floor(x), std::floor(y), std::floor(z)); 
	}

	CC_Vector3_Generic Ceil () const
	{
		return CC_Vector3_Generic(std::ceil(x), std::ceil(y), std::ceil(z)); 
	}

	CC_Vector3_Generic Max (const CC_Vector3_Generic& aVector)
	{
		return CC_Vector3_Generic(std::max(x, aVector.x), std::max(y, aVector.y), std::max(z, aVector.z));
	}

	CC_Vector3_Generic Min (const CC_Vector3_Generic& aVector)
	{
		return CC_Vector3_Generic(std::min(x, aVector.x), std::min(y, aVector.y), std::min(z, aVector.z));
	}

	CC_Vector3_Generic Catersian ()
	{
		return CC_Vector3_Generic(x * std::cos(z) * std::sin(y), x * std::sin(z) * std::sin(y), x * std::cos(y));
	}

	CC_Vector3_Generic Spherical ()
	{
		CC_Assert((x == 0 && y == 0), "Atan2 called with x and y equal to 0");
		return CC_Vector3_Generic(Magnitude2(), std::atan2(sqrtf(x*x + y*y), std::atan2(y,x)));
	}

};

template<class T, class U>
inline bool operator < (const CC_Vector3_Generic<T>& lhs, const CC_Vector3_Generic<U>& rhs)
{
	return (lhs.Magnitude() < rhs.Magnitude());
}

template<class T, class U>
inline bool operator > (const CC_Vector3_Generic<T>& lhs, const CC_Vector3_Generic<U>& rhs)
{
	return (lhs.Magnitude() > rhs.Magnitude());
}

#define CC_Vector3d_i CC_Vector3_Generic<int32_t>
#define CC_Vector3d_u CC_Vector3_Generic<uint32_t>
#define CC_Vector3d_f CC_Vector3_Generic<float>
#define CC_Vector3d_d CC_Vector3_Generic<double>

template<class T>
struct CC_Vector4_Generic
{
public:
	// constructors
	CC_Vector4_Generic()
		: x(0)
		, y(0)
		, z(0)
		, w(0)
	{}

	CC_Vector4_Generic(T _x, T _y, T _z, T _w)
		: x(_x)	
		, y(_y)
		, z(_z)
		, w(_w)
	{}

	CC_Vector4_Generic(const CC_Vector4_Generic& aVector)
		: x(aVector.x)
		, y(aVector.y)
		, z(aVector.z)
		, w(aVector.w)
	{}

	// members
	T x;
	T y;
	T z;
	T w;

	// operator overloads
	CC_Vector4_Generic& operator = (const CC_Vector4_Generic& aVector) = default;

	T operator [] (const uint aIndex) const
	{
		CC_Assert(aIndex < sizeof(CC_Vector4_Generic<T>)/sizeof(T), "Index is out of bounds");
		return *(T*)((T*)this + aIndex);
	}

	CC_Vector4_Generic operator + (const CC_Vector4_Generic& aVector) const
	{
		return CC_Vector4_Generic(x + aVector.x, y + aVector.y, z + aVector.z, w + aVector.w);
	}

	CC_Vector4_Generic operator - (const CC_Vector4_Generic& aVector) const
	{
		return CC_Vector4_Generic(x - aVector.x, y - aVector.y, z - aVector.z, w - aVector.w);
	}

	CC_Vector4_Generic operator * (const CC_Vector4_Generic& aVector) const
	{
		return CC_Vector4_Generic(x * aVector.x, y * aVector.y, z * aVector.z, w * aVector.w);
	}

	CC_Vector4_Generic operator * (const T rhs) const
	{
		return CC_Vector4_Generic(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	CC_Vector4_Generic operator / (const CC_Vector4_Generic& aVector) const
	{
		return CC_Vector4_Generic(x / aVector.x, y / aVector.y, z / aVector.z, w / aVector.w);
	}

	CC_Vector4_Generic operator / (const T rhs) const
	{
		return CC_Vector4_Generic(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	CC_Vector4_Generic operator += (const CC_Vector4_Generic& aVector) const
	{
		this->x += aVector.x;
		this->y += aVector.y;
		this->z += aVector.z;
		this->w += aVector.w;
		return *this;
	}

	CC_Vector4_Generic operator -= (const CC_Vector4_Generic& aVector) const
	{
		this->x -= aVector.x;
		this->y -= aVector.y;
		this->z -= aVector.z;
		this->z -= aVector.w;
		return *this;
	}

	CC_Vector4_Generic operator *= (const CC_Vector4_Generic& aVector) const
	{
		this->x *= aVector.x;
		this->y *= aVector.y;
		this->z *= aVector.z;
		this->w *= aVector.w;
		return *this;
	}

	CC_Vector4_Generic operator *= (const T rhs) const
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		this->w *= rhs;
		return *this;
	}

	CC_Vector4_Generic operator /= (const CC_Vector4_Generic& aVector) const
	{
		this->x /= aVector.x;
		this->y /= aVector.y;
		this->z /= aVector.z;
		this->w /= aVector.w;
		return *this;
	}

	CC_Vector4_Generic operator /= (const T rhs) const
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		this->w /= rhs;
		return *this;
	}

	CC_Vector4_Generic operator + () const
	{
		return CC_Vector3_Generic(+x, +y, +z, +w);
	}

	CC_Vector4_Generic operator - () const
	{
		return CC_Vector3_Generic(-x, -y, -z, -z);
	}

	bool operator == (const CC_Vector4_Generic& aVector) const
	{
		return (x == aVector.x && y == aVector.y && z == aVector.z && w == aVector.w);
	}

	bool operator != (const CC_Vector4_Generic& aVector) const
	{
		return (x != aVector.x || y != aVector.y || z != aVector.z || w != aVector.w);
	}

	// std::ostream support
	friend std::ostream& operator << (std::ostream& os, const CC_Vector4_Generic& rhs)
	{
		os << rhs.ToString();
		return os;
	}

	// utilities
	const std::string ToString() const
	{
			std::string s;
		s += "[";
		for (uint i = 0; i < 4; i++)
		{
			s += std::to_string(this[i]);
			s += ", ";
		}
		s += "]";
		return s;
	}

	// vector maths methods
	T Magnitude () const
	{
		return T(sqrtf(x*x + y*y + z*z + w*w));
	}

	T Magnitude2 () const
	{
		return (x*x + y*y + z*z + w*w);
	}

	CC_Vector4_Generic Normalized () const
	{
		T r = 1 / Magnitude2();
		return CC_Vector4_Generic(x*r, y*r, z*r, w*r);
	}

	T Dot (const CC_Vector4_Generic& aVector) const
	{
		return (x*aVector.x + y*aVector.y + z*aVector.z + w*aVector.w);
	}

	CC_Vector4_Generic Cross (const CC_Vector4_Generic& aVector) const
	{
		return CC_Vector3_Generic(y*aVector.z - z*aVector.y, x*aVector.z - z*aVector.x, x*aVector.y - y*aVector.x);
	}

	// vector maths extension
	
	CC_Vector4_Generic Floor () const
	{
		return CC_Vector3_Generic(std::floor(x), std::floor(y), std::floor(z), std::floor(w)); 
	}

	CC_Vector4_Generic Ceil () const
	{
		return CC_Vector3_Generic(std::ceil(x), std::ceil(y), std::ceil(z), std::ceil(w)); 
	}

	CC_Vector4_Generic Max (const CC_Vector4_Generic& aVector)
	{
		return CC_Vector3_Generic(std::max(x, aVector.x), std::max(y, aVector.y), std::max(z, aVector.z), std::max(w, aVector.w));
	}

	CC_Vector4_Generic Min (const CC_Vector4_Generic& aVector)
	{
		return CC_Vector3_Generic(std::min(x, aVector.x), std::min(y, aVector.y), std::min(z, aVector.z), std::min(w, aVector.w));
	}
};

template<class T, class U>
inline bool operator < (const CC_Vector4_Generic<T>& lhs, const CC_Vector4_Generic<U>& rhs)
{
	return (lhs.Magnitude() < rhs.Magnitude());
}

template<class T, class U>
inline bool operator > (const CC_Vector4_Generic<T>& lhs, const CC_Vector4_Generic<U>& rhs)
{
	return (lhs.Magnitude() > rhs.Magnitude());
}

#define CC_Vector4d_i CC_Vector4_Generic<int32_t>
#define CC_Vector4d_u CC_Vector4_Generic<uint32_t>
#define CC_Vector4d_f CC_Vector4_Generic<float>
#define CC_Vector4d_d CC_Vector4_Generic<double>

