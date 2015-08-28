#include "Math/Mat.h"
#include <cstdio>
#include <emmintrin.h>

Vec3f operator*(const Mat3 &l, const Vec3f &r)
{
	return {
		l[0] * r[0] + l[3] * r[1] + l[6] * r[2],
		l[1] * r[0] + l[4] * r[1] + l[7] * r[2],
		l[2] * r[0] + l[5] * r[1] + l[8] * r[2],
	};
}

Vec3f operator*(const Vec3f &l, const Mat3 &r)
{
	return {
		l[0] * r[0] + l[1] * r[1] + l[2] * r[2],
		l[0] * r[3] + l[1] * r[4] + l[2] * r[5],
		l[0] * r[6] + l[1] * r[7] + l[2] * r[8],
	};
}

void Mat4::dump() const
{
	printf("[%f %f %f %f\n",  data[0], data[4], data[8],  data[12]);
	printf(" %f %f %f %f\n",  data[1], data[5], data[9],  data[13]);
	printf(" %f %f %f %f\n",  data[2], data[6], data[10], data[14]);
	printf(" %f %f %f %f]\n", data[3], data[7], data[11], data[15]);
}

bool operator==(const Mat4 &l, const Mat4 &r)
{
	return (
		l[0]  == r[0] &&
		l[1]  == r[1] &&
		l[2]  == r[2] &&
		l[3]  == r[3] &&
		l[4]  == r[4] &&
		l[5]  == r[5] &&
		l[6]  == r[6] &&
		l[7]  == r[7] &&
		l[8]  == r[8] &&
		l[9]  == r[9] &&
		l[10] == r[10] &&
		l[11] == r[11] &&
		l[12] == r[12] &&
		l[13] == r[13] &&
		l[14] == r[14] &&
		l[15] == r[15]
	);
}

bool operator!=(const Mat4 &lhs, const Mat4 &rhs)
{
	return !operator==(lhs, rhs);
}

Mat4 operator*(const Mat4 &l, const Mat4 &r)
{
	Mat4 out;
	out[0]  = l[0] * r[0]  + l[4] * r[1]  + l[8]  * r[2]  + l[12] * r[3];
	out[1]  = l[1] * r[0]  + l[5] * r[1]  + l[9]  * r[2]  + l[13] * r[3];
	out[2]  = l[2] * r[0]  + l[6] * r[1]  + l[10] * r[2]  + l[14] * r[3];
	out[3]  = l[3] * r[0]  + l[7] * r[1]  + l[11] * r[2]  + l[15] * r[3];

	out[4]  = l[0] * r[4]  + l[4] * r[5]  + l[8]  * r[6]  + l[12] * r[7];
	out[5]  = l[1] * r[4]  + l[5] * r[5]  + l[9]  * r[6]  + l[13] * r[7];
	out[6]  = l[2] * r[4]  + l[6] * r[5]  + l[10] * r[6]  + l[14] * r[7];
	out[7]  = l[3] * r[4]  + l[7] * r[5]  + l[11] * r[6]  + l[15] * r[7];

	out[8]  = l[0] * r[8]  + l[4] * r[9]  + l[8]  * r[10] + l[12] * r[11];
	out[9]  = l[1] * r[8]  + l[5] * r[9]  + l[9]  * r[10] + l[13] * r[11];
	out[10] = l[2] * r[8]  + l[6] * r[9]  + l[10] * r[10] + l[14] * r[11];
	out[11] = l[3] * r[8]  + l[7] * r[9]  + l[11] * r[10] + l[15] * r[11];

	out[12] = l[0] * r[12] + l[4] * r[13] + l[8]  * r[14] + l[12] * r[15];
	out[13] = l[1] * r[12] + l[5] * r[13] + l[9]  * r[14] + l[13] * r[15];
	out[14] = l[2] * r[12] + l[6] * r[13] + l[10] * r[14] + l[14] * r[15];
	out[15] = l[3] * r[12] + l[7] * r[13] + l[11] * r[14] + l[15] * r[15];
	return out;
}

Mat4 operator+(const Mat4 &lhs, const Mat4 &rhs)
{
	Mat4 out;
	out[0]  = lhs[0]  + rhs[0];
	out[1]  = lhs[1]  + rhs[1];
	out[2]  = lhs[2]  + rhs[2];
	out[3]  = lhs[3]  + rhs[3];
	out[4]  = lhs[4]  + rhs[4];
	out[5]  = lhs[5]  + rhs[5];
	out[6]  = lhs[6]  + rhs[6];
	out[7]  = lhs[7]  + rhs[7];
	out[8]  = lhs[8]  + rhs[8];
	out[9]  = lhs[9]  + rhs[9];
	out[10] = lhs[10] + rhs[10];
	out[11] = lhs[11] + rhs[11];
	out[12] = lhs[12] + rhs[12];
	out[13] = lhs[13] + rhs[13];
	out[14] = lhs[14] + rhs[14];
	out[15] = lhs[15] + rhs[15];
	return out;
}

Vec3f operator*(const Mat4 &l, const Vec3f &r)
{
	return {
		l[0] * r[0] + l[4] * r[1] + l[8]  * r[2] + l[12],
		l[1] * r[0] + l[5] * r[1] + l[9]  * r[2] + l[13],
		l[2] * r[0] + l[6] * r[1] + l[10] * r[2] + l[14]
	};
}

Vec3f operator*(const Vec3f &l, const Mat4 &r)
{
	return {
		l[0] * r[0]  + l[1] * r[1]  + l[2] * r[2]  + r[3],
		l[0] * r[4]  + l[1] * r[5]  + l[2] * r[6]  + r[7],
		l[0] * r[8]  + l[1] * r[9]  + l[2] * r[10] + r[11],
	};
}

// matrix l by column-vector r
Vec4f operator*(const Mat4 &l, const Vec4f &r)
{
	return {
		l[0] * r[0] + l[4] * r[1] + l[8]  * r[2] + l[12] * r[3],
		l[1] * r[0] + l[5] * r[1] + l[9]  * r[2] + l[13] * r[3],
		l[2] * r[0] + l[6] * r[1] + l[10] * r[2] + l[14] * r[3],
		l[3] * r[0] + l[7] * r[1] + l[11] * r[2] + l[15] * r[3]
	};
}

// row-vector by matrix r
Vec4f operator*(const Vec4f &l, const Mat4 &r)
{
	return {
		l[0] * r[0]  + l[1] * r[1]  + l[2] * r[2]  + l[3] * r[3],
		l[0] * r[4]  + l[1] * r[5]  + l[2] * r[6]  + l[3] * r[7],
		l[0] * r[8]  + l[1] * r[9]  + l[2] * r[10] + l[3] * r[11],
		l[0] * r[12] + l[1] * r[13] + l[2] * r[14] + l[3] * r[15]
	};
}

Mat4 Mat4_Rotate(const Vec3f &axis, float angle)
{
	Mat4 m;
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	Vec3f v = normalize(axis);
	float xx = v.x * v.x;
	float yy = v.y * v.y;
	float zz = v.z * v.z;
	float xy = v.x * v.y;
	float yz = v.y * v.z;
	float zx = v.z * v.x;
	float xs = v.x * s;
	float ys = v.y * s;
	float zs = v.z * s;
	m[0] = (1.0f - c) * xx + c;  m[4] = (1.0f - c) * xy - zs; m[8]  = (1.0f - c) * zx + ys; m[12] = 0.0f;
	m[1] = (1.0f - c) * xy + zs; m[5] = (1.0f - c) * yy + c;  m[9]  = (1.0f - c) * yz - xs; m[13] = 0.0f;
	m[2] = (1.0f - c) * zx - ys; m[6] = (1.0f - c) * yz + xs; m[10] = (1.0f - c) * zz + c;  m[14] = 0.0f;
	m[3] = 0.0f;                 m[7] = 0.0f;                 m[11] = 0.0f;                 m[15] = 1.0f;

	return m;
}

Mat4 Mat4_RotateX(float angle)
{
	Mat4 m;
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	m[0] = 1.0f; m[4] = 0.0f; m[8]  = 0.0f; m[12] = 0.0f;
	m[1] = 0.0f; m[5] = c;    m[9]  = -s;   m[13] = 0.0f;
	m[2] = 0.0f; m[6] = s;    m[10] = c;    m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;

	return m;
}

Mat4 Mat4_RotateY(float angle)
{
	Mat4 m;
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	m[0] = c;    m[4] = 0.0f; m[8]  = s;    m[12] = 0.0f;
	m[1] = 0.0f; m[5] = 1.0f; m[9]  = 0.0f; m[13] = 0.0f;
	m[2] = -s;   m[6] = 0.0f; m[10] = c;    m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;

	return m;
}

Mat4 Mat4_RotateZ(float angle)
{
	Mat4 m;
	float rad = angle * MATH_DEG_TO_RAD;
	float c = cosf(rad);
	float s = sinf(rad);
	m[0] = c;    m[4] = -s;   m[8]  = 0.0f; m[12] = 0.0f;
	m[1] = s;    m[5] = c;    m[9]  = 0.0f; m[13] = 0.0f;
	m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;

	return m;
}

Mat4 Mat4_Scale(const Vec3f &v)
{
	Mat4 m;
	m[0] = v.x;  m[4] = 0.0f; m[8]  = 0.0f; m[12] = 0.0f;
	m[1] = 0.0f; m[5] = v.y;  m[9]  = 0.0f; m[13] = 0.0f;
	m[2] = 0.0f; m[6] = 0.0f; m[10] = v.z;  m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;

	return m;
}

Mat4 Mat4_Scale(float f)
{
	return Mat4_Scale(Vec3f(f));
}

Mat4 Mat4_Translate(const Vec3f &v)
{
	Mat4 m;
	m[0] = 1.0f; m[4] = 0.0f; m[8]  = 0.0f; m[12] = v.x;
	m[1] = 0.0f; m[5] = 1.0f; m[9]  = 0.0f; m[13] = v.y;
	m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = v.z;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;

	return m;
}

Mat4 Mat4_Perspective(float fov, float aspect, float znear, float zfar)
{
	Mat4 m;
	float y = std::tan(fov * MATH_DEG_TO_RAD / 2);
	float x = y * aspect;

	m[0] = 1.0f / x;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = 1.0f / y;
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = -(zfar + znear) / (zfar - znear);
	m[11] = -1.0f;

	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -(2.0f * zfar * znear) / (zfar - znear);
	m[15] = 0.0f;

	return m;
}

Mat4 Mat4_Ortho(float left, float right, float bottom, float top, float znear, float zfar)
{
	Mat4 m;
	float x = 2.0f / (right - left);
	float y = 2.0f / (top - bottom);
	float z = -2.0f / (zfar - znear);
	float tx = - ((right + left) / (right - left));
	float ty = - ((top + bottom) / (top - bottom));
	float tz = - ((zfar + znear) / (zfar - znear));

	m[0] = x;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;

	m[4] = 0.0f;
	m[5] = y;
	m[6] = 0.0f;
	m[7] = 0.0f;

	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = z;
	m[11] = 0.0f;

	m[12] = tx;
	m[13] = ty;
	m[14] = tz;
	m[15] = 1.0f;

	return m;
}

Mat4 Mat4_LookAt(const Vec3f &eye, const Vec3f &center, const Vec3f &up)
{
	Mat4 m;
	Vec3f n,u,s;
	n = normalize(eye - center);
	s = normalize(cross(up, n));
	u = normalize(cross(n, s));

	m[0] = s.x;  m[4] = s.y;  m[8]  = s.z;  m[12] = 0.0f;
	m[1] = u.x;  m[5] = u.y;  m[9]  = u.z;  m[13] = 0.0f;
	m[2] = n.x;  m[6] = n.y;  m[10] = n.z;  m[14] = 0.0f;
	m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
	return m * Mat4_Translate(-eye);
}

Mat4 transpose(const Mat4 &m)
{
	Mat4 r;
	r[0] = m[0];  r[4] = m[1];  r[8]  = m[2];  r[12] = m[3];
	r[1] = m[4];  r[5] = m[5];  r[9]  = m[6];  r[13] = m[7];
	r[2] = m[8];  r[6] = m[9];  r[10] = m[10]; r[14] = m[11];
	r[3] = m[12]; r[7] = m[13]; r[11] = m[14]; r[15] = m[15];
	return r;
}

float determinant(const Mat4 &m)
{
	float d;
	d =  m[0]  * (m[5] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[6] * m[15] - m[14] * m[7]) + m[13] * (m[6] * m[11] - m[10] * m[7]));
	d -= m[4]  * (m[1] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[11] - m[10] * m[3]));
	d += m[8]  * (m[1] * (m[6]  * m[15] - m[14] * m[7])  - m[5] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[7]  - m[6]  * m[3]));
	d -= m[12] * (m[1] * (m[6]  * m[11] - m[10] * m[7])  - m[5] * (m[2] * m[11] - m[10] * m[3]) + m[9]  * (m[2] * m[7]  - m[6]  * m[3]));
	return d;
}

Mat4 inverse(const Mat4 &m, bool *inversed)
{
	Mat4 r;
	float d = determinant(m);
	if (d < MATH_EPSILON) {
		if (inversed)
			*inversed = false;
		return r;
	}
	float id = 1.0f / d;
	r[0]  =  (m[5] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[6] * m[15] - m[14] * m[7]) + m[13] * (m[6] * m[11] - m[10] * m[7])) * id;
	r[1]  = -(m[1] * (m[10] * m[15] - m[14] * m[11]) - m[9] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[11] - m[10] * m[3])) * id;
	r[2]  =  (m[1] * (m[6]  * m[15] - m[14] * m[7])  - m[5] * (m[2] * m[15] - m[14] * m[3]) + m[13] * (m[2] * m[7]  - m[6]  * m[3])) * id;
	r[3]  = -(m[1] * (m[6]  * m[11] - m[10] * m[7])  - m[5] * (m[2] * m[11] - m[10] * m[3]) + m[9]  * (m[2] * m[7]  - m[6]  * m[3])) * id;
	r[4]  = -(m[4] * (m[10] * m[15] - m[14] * m[11]) - m[8] * (m[6] * m[15] - m[14] * m[7]) + m[12] * (m[6] * m[11] - m[10] * m[7])) * id;
	r[5]  =  (m[0] * (m[10] * m[15] - m[14] * m[11]) - m[8] * (m[2] * m[15] - m[14] * m[3]) + m[12] * (m[2] * m[11] - m[10] * m[3])) * id;
	r[6]  = -(m[0] * (m[6]  * m[15] - m[14] * m[7])  - m[4] * (m[2] * m[15] - m[14] * m[3]) + m[12] * (m[2] * m[7]  - m[6]  * m[3])) * id;
	r[7]  =  (m[0] * (m[6]  * m[11] - m[10] * m[7])  - m[4] * (m[2] * m[11] - m[10] * m[3]) + m[8]  * (m[2] * m[7]  - m[6]  * m[3])) * id;
	r[8]  =  (m[4] * (m[9]  * m[15] - m[13] * m[11]) - m[8] * (m[5] * m[15] - m[13] * m[7]) + m[12] * (m[5] * m[11] - m[9]  * m[7])) * id;
	r[9]  = -(m[0] * (m[9]  * m[15] - m[13] * m[11]) - m[8] * (m[1] * m[15] - m[13] * m[3]) + m[12] * (m[1] * m[11] - m[9]  * m[3])) * id;
	r[10] =  (m[0] * (m[5]  * m[15] - m[13] * m[7])  - m[4] * (m[1] * m[15] - m[13] * m[3]) + m[12] * (m[1] * m[7]  - m[5]  * m[3])) * id;
	r[11] = -(m[0] * (m[5]  * m[11] - m[9]  * m[7])  - m[4] * (m[1] * m[11] - m[9]  * m[3]) + m[8]  * (m[1] * m[7]  - m[5]  * m[3])) * id;
	r[12] = -(m[4] * (m[9]  * m[14] - m[13] * m[10]) - m[8] * (m[5] * m[14] - m[13] * m[6]) + m[12] * (m[5] * m[10] - m[9]  * m[6])) * id;
	r[13] =  (m[0] * (m[9]  * m[14] - m[13] * m[10]) - m[8] * (m[1] * m[14] - m[13] * m[2]) + m[12] * (m[1] * m[10] - m[9]  * m[2])) * id;
	r[14] = -(m[0] * (m[5]  * m[14] - m[13] * m[6])  - m[4] * (m[1] * m[14] - m[13] * m[2]) + m[12] * (m[1] * m[6]  - m[5]  * m[2])) * id;
	r[15] =  (m[0] * (m[5]  * m[10] - m[9]  * m[6])  - m[4] * (m[1] * m[10] - m[9]  * m[2]) + m[8]  * (m[1] * m[6]  - m[5]  * m[2])) * id;
	if (inversed)
		*inversed = true;
	return r;
}

Vec4f Vec4_MiniOrtho2D(float left, float right, float bottom, float top, const Vec2f &offset)
{
	float x = 2.0f / (right - left);
	float y = 2.0f / (top - bottom);
	float tx = -((right + left) / (right - left)) + offset.x * x;
	float ty = -((top + bottom) / (top - bottom)) + offset.y * y;
	return {x, y, tx, ty};
}

Vec4f mini_ortho_translate(const Vec4f &miniortho, const Vec2f &offset)
{
	return {
		miniortho.x,
		miniortho.y,
		miniortho.z + miniortho.x * offset.x,
		miniortho.w + miniortho.y * offset.y
	};
}

Vec4f Vec4_MiniPerspective3D(float vfov, float aspect, float znear, float zfar)
{
	float v = std::tan(vfov * MATH_DEG_TO_RAD);
	float h = v * aspect;
	return {
		1.0f / v,
		1.0f / h,
		-(zfar + znear) / (zfar - znear),
		-(2.0f * zfar * znear) / (zfar - znear)
	};
}

Mat4 to_mat4(const Mat3 &m)
{
	return Mat4(
		m[0], m[1], m[2], 0,
		m[3], m[4], m[5], 0,
		m[6], m[7], m[8], 0,
		0,    0,    0,    1
	);
}

Mat3 to_mat3(const Mat4 &m)
{
	return Mat3(
		m[0], m[1], m[2],
		m[4], m[5], m[6],
		m[8], m[9], m[10]
	);
}
