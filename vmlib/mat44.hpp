#ifndef MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
#define MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA

#include <cmath>
#include <cassert>
#include <cstdlib>

#include "vec3.hpp"
#include "vec4.hpp"

/** Mat44f: 4x4 matrix with floats
 *
 * See vec2f.hpp for discussion. Similar to the implementation, the Mat44f is
 * intentionally kept simple and somewhat bare bones.
 *
 * The matrix is stored in row-major order (careful when passing it to OpenGL).
 *
 * The overloaded operator () allows access to individual elements. Example:
 *    Mat44f m = ...;
 *    float m12 = m(1,2);
 *    m(0,3) = 3.f;
 *
 * The matrix is arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
struct Mat44f
{
	float v[16];

	constexpr
	float& operator() (std::size_t aI, std::size_t aJ) noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
	constexpr
	float const& operator() (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert( aI < 4 && aJ < 4 );
		return v[aI*4 + aJ];
	}
};

// Identity matrix
constexpr Mat44f kIdentity44f = { {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
} };

// Common operators for Mat44f.
// Note that you will need to implement these yourself.

constexpr
Mat44f operator*( Mat44f const& aLeft, Mat44f const& aRight ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation

	// Perform matrix multiplication
	Mat44f result = kIdentity44f;

	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 4; ++j) {
			result(i, j) = 0.f;
			for (std::size_t k = 0; k < 4; ++k) {
				result(i, j) += aLeft(i, k) * aRight(k, j);
			}
		}
	}

	return result;
}

constexpr
Vec4f operator*( Mat44f const& aLeft, Vec4f const& aRight ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	Vec4f result = { 0.f, 0.f, 0.f, 0.f };

	result.x = aLeft(0, 0) * aRight.x + aLeft(0, 1) * aRight.y + aLeft(0, 2) * aRight.z + aLeft(0, 3) * aRight.w;
	result.y = aLeft(1, 0) * aRight.x + aLeft(1, 1) * aRight.y + aLeft(1, 2) * aRight.z + aLeft(1, 3) * aRight.w;
	result.z = aLeft(2, 0) * aRight.x + aLeft(2, 1) * aRight.y + aLeft(2, 2) * aRight.z + aLeft(2, 3) * aRight.w;
	result.w = aLeft(3, 0) * aRight.x + aLeft(3, 1) * aRight.y + aLeft(3, 2) * aRight.z + aLeft(3, 3) * aRight.w;

	return result;
}

// Functions:

Mat44f invert( Mat44f const& aM ) noexcept;

inline
Mat44f transpose( Mat44f const& aM ) noexcept
{
	Mat44f ret;
	for( std::size_t i = 0; i < 4; ++i )
	{
		for( std::size_t j = 0; j < 4; ++j )
			ret(j,i) = aM(i,j);
	}
	return ret;
}

inline
Mat44f make_rotation_x( float aAngle ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	 
	Mat44f res;

	float c = std::cos(aAngle);
	float s = std::sin(aAngle);

	res.v[0] = 1.0f;
	res.v[1] = 0.0f;
	res.v[2] = 0.0f;
	res.v[3] = 0.0f;
	res.v[4] = 0.0f;
	res.v[5] = c;
	res.v[6] = -s;
	res.v[7] = 0.0f;
	res.v[8] = 0.0f;
	res.v[9] = s;
	res.v[10] = c;
	res.v[11] = 0.0f;
	res.v[12] = 0.0f;
	res.v[13] = 0.0f;
	res.v[14] = 0.0f;
	res.v[15] = 1.0f;

	return res;
}


inline
Mat44f make_rotation_y( float aAngle ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	Mat44f res;

	float c = std::cos(aAngle);
	float s = std::sin(aAngle);

	res.v[0] = c;
	res.v[1] = 0.0f;
	res.v[2] = s;
	res.v[3] = 0.0f;
	res.v[4] = 0.0f;
	res.v[5] = 1.0f;
	res.v[6] = 0.0f;
	res.v[7] = 0.0f;
	res.v[8] = -s;
	res.v[9] = 0.0f;
	res.v[10] = c;
	res.v[11] = 0.0f;
	res.v[12] = 0.0f;
	res.v[13] = 0.0f;
	res.v[14] = 0.0f;
	res.v[15] = 1.0f;

	return res;
}

inline
Mat44f make_rotation_z( float aAngle ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	Mat44f res;

	float c = std::cos(aAngle);
	float s = std::sin(aAngle);

	res.v[0] = c;
	res.v[1] = -s;
	res.v[2] = 0.0f;
	res.v[3] = 0.0f;
	res.v[4] = s;
	res.v[5] = c;
	res.v[6] = 0.0f;
	res.v[7] = 0.0f;
	res.v[8] = 0.0f;
	res.v[9] = 0.0f;
	res.v[10] = 1.0f;
	res.v[11] = 0.0f;
	res.v[12] = 0.0f;
	res.v[13] = 0.0f;
	res.v[14] = 0.0f;
	res.v[15] = 1.0f;

	return res;
}

inline
Mat44f make_translation( Vec3f aTranslation ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	Mat44f res;

	for (int i = 0; i < 16; ++i) {
		res.v[i] = 0.0f;
	}
	
	res.v[0] = 1.0f;
	res.v[5] = 1.0f;
	res.v[10] = 1.0f;
	res.v[15] = 1.0f;

	res.v[3] = aTranslation.x;
	res.v[7] = aTranslation.y;
	res.v[11] = aTranslation.z;

	return res;
}

inline
Mat44f make_scaling( float aSX, float aSY, float aSZ ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	(void)aSX;  // Avoid warnings about unused arguments until the function
	(void)aSY;  // is properly implemented.
	(void)aSZ;
	return kIdentity44f;
}


inline
Mat44f make_perspective_projection( float aFovInRadians, float aAspect, float aNear, float aFar ) noexcept
{
	//TODO: your implementation goes here
	//TODO: remove the following when you start your implementation
	Mat44f res;

	for (int i = 0; i < 16; ++i) {
		res.v[i] = 0.0f;
	}

	res.v[0] = 1.0f / (aAspect * std::tan(aFovInRadians / 2.0f));
	res.v[5] = 1.0f / std::tan(aFovInRadians / 2.0f);
	res.v[10] = -(aFar + aNear) / (aNear - aFar);
	res.v[11] = -(2.0f * aFar * aNear) / (aNear - aFar);
	res.v[14] = -1.0f;
	

	
	return res;
}




#endif // MAT44_HPP_E7187A26_469E_48AD_A3D2_63150F05A4CA
