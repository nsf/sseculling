#pragma once

#include "Core/Slice.h"
#include "Math/Plane.h"
#include "Math/Quat.h"
#include "Math/Vec.h"
#include "Math/Sphere.h"
#include "Math/Transform.h"
#include "Math/Mat.h"

enum FrustumSide {
	FS_INSIDE,
	FS_OUTSIDE,
	FS_BOTH,
};

enum FrustumPlane {
	FP_FAR,
	FP_NEAR,
	FP_LEFT,
	FP_RIGHT,
	FP_BOTTOM,
	FP_TOP,
};

enum FrustumPlaneCorner {
	FPC_TOP_LEFT,
	FPC_TOP_RIGHT,
	FPC_BOTTOM_LEFT,
	FPC_BOTTOM_RIGHT,
};

enum FrustumCullingType {
	FCT_NORMAL,
	FCT_NO_NEAR_PLANE,
};

struct Frustum {
	Frustum() = default;
	void update_planes();

	FrustumSide cull(const Vec3f &min, const Vec3f &max, FrustumCullingType type = FCT_NORMAL) const;

	// returns true only if the sphere is outside of the frustum
	bool cull(const Sphere &s) const;

	Plane planes[6];
	Vec3f near[4];
	Vec3f far[4];

	Sphere bounding_sphere() const;
};

Frustum Frustum_Ortho(float left, float right, float bottom, float top, float znear, float zfar);
Frustum Frustum_Perspective(float fov, float aspect, float znear, float zfar);
Frustum Frustum_Shadow(const Sphere &bsphere, float zextend);
Mat4 Mat4_Shadow(const Sphere &bsphere, float zextend);

// Returns top left, top right, bottom left, bottom right corners.
void frustum_plane_corners(Vec3f out[4], float fov, float aspect, float plane);
Vec2f frustum_plane_wh(float fov, float aspect, float plane);
Vec2f projection_ratio(float near, float far);
Frustum transform(const Frustum &frustum_src, const Transform &tr);
void generate_frustum_split_spheres(
	Slice<Sphere> spheres_out,
	float fov, float aspect, float znear, float zfar, float ratio);
