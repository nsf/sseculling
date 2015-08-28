#pragma once

#include "Math/Vec.h"
#include "Math/Mat.h"

//------------------------------------------------------------------------------
// Quat
//------------------------------------------------------------------------------

struct Quat {
	union {
		struct {
			float x, y, z, w;
		};
		float data[4];
	};

	Quat() = default;
	Quat(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}
	Quat(float x, float y, float z): x(x), y(y), z(z) { reconstruct_w(); }

	// dir should be normalized, angle is in degrees
	// Represents CCW rotation around dir, when dir points at viewer and
	// coordinate system is RH.
	Quat(const Vec3f &dir, float angle);

	// both u and v should be normalized
	Quat(const Vec3f &u, const Vec3f &v);

	// rotates the vector 'v' by quaternion
	Vec3f rotate(const Vec3f &v) const;

	void reconstruct_w() { w = std::sqrt(std::abs(1.0f - (x*x + y*y + z*z))); }

	Vec3f compress() const { return w < 0.0 ? Vec3f(-x, -y, -z) : Vec3f(x, y, z); }

	float &operator[](int i) { return data[i]; }
	float operator[](int i) const { return data[i]; }
	Quat operator-() const { return Quat(-x, -y, -z, -w); }
};

static inline bool operator==(const Quat &l, const Quat &r)
{
	return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w;
}

static inline bool operator!=(const Quat &l, const Quat &r)
{
	return l.x != r.x || l.y != r.y || l.z != r.z || l.w != r.w;
}

// Multiplying l with r applies the rotation r to l
static inline Quat operator*(const Quat &l, const Quat &r)
{
	return {
		l.w * r.x + l.x * r.w + l.y * r.z - l.z * r.y,
		l.w * r.y + l.y * r.w + l.z * r.x - l.x * r.z,
		l.w * r.z + l.z * r.w + l.x * r.y - l.y * r.x,
		l.w * r.w - l.x * r.x - l.y * r.y - l.z * r.z
	};
}

Quat normalize(const Quat &q);
Quat slerp(const Quat &q0, const Quat &q1, float t);

Quat to_quat(const Mat4 &m);
Mat3 to_mat3(const Quat &q);
Mat4 to_mat4(const Quat &q);

static inline Quat inverse(const Quat &q) { return {-q.x, -q.y, -q.z, q.w}; }

static inline Quat Quat_Identity() { return {0, 0, 0, 1}; }
Quat Quat_LookAt(const Vec3f &dir);
