#include "Math/Quat.h"

Quat::Quat(const Vec3f &dir, float angle)
{
	const float halfangle = angle * MATH_DEG_TO_RAD / 2.0f;
	const float sinangle = sinf(halfangle);
	x = dir.x * sinangle;
	y = dir.y * sinangle;
	z = dir.z * sinangle;
	w = cosf(halfangle);
}

Quat::Quat(const Vec3f &u, const Vec3f &v)
{
	Vec3f w = cross(u, v);
	float len2 = length2(w);
	float real = dot(u, v);
	if (len2 < MATH_EPSILON && real < 0) {
		w = std::abs(u.x) > std::abs(u.z) ?
			Vec3f(-u.y, u.x, 0) / Vec3f(length(u.XY())) :
			Vec3f(0, -u.z, u.y) / Vec3f(length(u.YZ()));
		this->x = w.x;
		this->y = w.y;
		this->z = w.z;
		this->w = 0;
		return;
	}

	real += std::sqrt(real * real + len2);
	float ilen = 1 / std::sqrt(real * real + len2);
	this->x = w.x * ilen;
	this->y = w.y * ilen;
	this->z = w.z * ilen;
	this->w = real * ilen;
}

Vec3f Quat::rotate(const Vec3f &v) const
{
	/*
	// Alternative implementation, don't know which one is faster.
	// TODO: measure both
	const Vec3 xyz(x, y, z);
	const Vec3 t = Vec3(2) * Cross(xyz, v);
	return v + Vec3(w) * t + Cross(xyz, t);
	*/
	const Vec3f xyz(x, y, z);
	return v + Vec3f(2) * cross(xyz, cross(xyz, v) + Vec3f(w) * v);
}

Quat normalize(const Quat &q)
{
	float il = 1.0f / (q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
	return {q.x * il, q.y * il, q.z * il, q.w * il};
}

Quat slerp(const Quat &q0, const Quat &q1, float t)
{
	float k0, k1, cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
	Quat q;
	if (cosomega < 0.0f) {
		cosomega = -cosomega;
		q.x = -q1.x;
		q.y = -q1.y;
		q.z = -q1.z;
		q.w = -q1.w;
	} else {
		q.x = q1.x;
		q.y = q1.y;
		q.z = q1.z;
		q.w = q1.w;
	}
	if (1.0f - cosomega > MATH_EPSILON) {
		float omega = acosf(cosomega);
		float sinomega = sinf(omega);
		k0 = sinf((1.0f - t) * omega) / sinomega;
		k1 = sinf(t * omega) / sinomega;
	} else {
		k0 = 1.0f - t;
		k1 = t;
	}

	return {
		q0.x * k0 + q.x * k1,
		q0.y * k0 + q.y * k1,
		q0.z * k0 + q.z * k1,
		q0.w * k0 + q.w * k1
	};
}

Quat to_quat(const Mat4 &m)
{
	Quat q;
	float trace = m[0] + m[5] + m[10];
	if (trace > 0.0f) {
		float s = sqrtf(trace + 1.0f);
		q[3] = 0.5f * s;
		s = 0.5f / s;
		q[0] = (m[6] - m[9]) * s;
		q[1] = (m[8] - m[2]) * s;
		q[2] = (m[1] - m[4]) * s;
	} else {
		static const int next[3] = { 1, 2, 0 };
		int i = 0;
		if (m[5] > m[0]) i = 1;
		if (m[10] > m[4 * i + i]) i = 2;
		int j = next[i];
		int k = next[j];
		float s = sqrtf(m[4 * i + i] - m[4 * j + j] - m[4 * k + k] + 1.0f);
		q[i] = 0.5f * s;
		if (s != 0) s = 0.5f / s;
		q[3] = (m[4 * j + k] - m[4 * k + j]) * s;
		q[j] = (m[4 * i + j] + m[4 * j + i]) * s;
		q[k] = (m[4 * i + k] + m[4 * k + i]) * s;
	}
	return q;
}

Mat3 to_mat3(const Quat &q)
{
	Mat3 r;
	const float x2 = q.x + q.x;
	const float y2 = q.y + q.y;
	const float z2 = q.z + q.z;
	const float xx = q.x * x2;
	const float yy = q.y * y2;
	const float zz = q.z * z2;
	const float xy = q.x * y2;
	const float yz = q.y * z2;
	const float xz = q.z * x2;
	const float wx = q.w * x2;
	const float wy = q.w * y2;
	const float wz = q.w * z2;
	r[0] = 1.0f - (yy + zz); r[3] = xy - wz;          r[6] = xz + wy;
	r[1] = xy + wz;          r[4] = 1.0f - (xx + zz); r[7] = yz - wx;
	r[2] = xz - wy;          r[5] = yz + wx;          r[8] = 1.0f - (xx + yy);
	return r;
}

Mat4 to_mat4(const Quat &q)
{
	return to_mat4(to_mat3(q));
}

Quat Quat_LookAt(const Vec3f &v)
{
	const Vec3f unit = -Vec3f_Z();
	const Vec3f around_y = normalize(Vec3f(v.x, 0, v.z));
	const Quat xq(unit, around_y);
	const Quat yq(around_y, v);
	return yq * xq;
}
