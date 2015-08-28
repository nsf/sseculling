#pragma once

#include "Math/Vec.h"
#include "Math/Quat.h"

struct Transform {
	Quat orientation = Quat_Identity();
	Vec3f translation = Vec3f(0);
	float _pad;

	Transform() = default;
	Transform(const Quat &orientation, const Vec3f &translation):
		orientation(orientation), translation(translation)
	{
	}

	explicit Transform(const Quat &orientation): orientation(orientation) {}
	explicit Transform(const Vec3f &translation): translation(translation) {}
};

Transform inverse(const Transform &tf);
Mat4 to_mat4(const Transform &tf);

Vec3f transform(const Vec3f &in, const Transform &tr);
Transform transform(const Transform &in, const Transform &tr);
