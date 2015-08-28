#pragma once

#include "Math/Vec.h"
#include "Math/Sphere.h"

enum PlaneSide {
	PS_FRONT,
	PS_BACK,
	PS_BOTH,
};

struct Plane {
	Plane() = default;

	// the normal should be normalized
	Plane(const Vec3f &origin, const Vec3f &normal): n(normal), d(-dot(n, origin)) {}
	Plane(const Vec3f &v1, const Vec3f &v2, const Vec3f &v3)
	{
		n = normalize(cross(v2 - v1, v3 - v1));
		d = -dot(n, v1);
	}

	bool operator==(const Plane &r) const { return n == r.n && d == r.d; }
	bool operator!=(const Plane &r) const { return n != r.n || d != r.d; }

	PlaneSide side(const Vec3f &point) const;
	PlaneSide side(const Vec3f &min, const Vec3f &max) const;

	Vec3f n;
	float d;
};
