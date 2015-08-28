#pragma once

#include <cmath>
#include <cstdlib>
#include <cstdint>
#include "Math/Utils.h"

//------------------------------------------------------------------------------
// Vec2
//------------------------------------------------------------------------------


#define _DEFINE_VEC2_NO_FUNCTIONS(type, Vec2)
#define _DEFINE_VEC2_NO_MEMBERS(type, Vec2)


#define _DEFINE_VEC2_INT_MEMBERS(type, Vec2)                          \
	Vec2 &operator|=(const Vec2 &r) { x|=r.x; y|=r.y; return *this; } \
	Vec2 &operator&=(const Vec2 &r) { x&=r.x; y&=r.y; return *this; } \
	Vec2 &operator^=(const Vec2 &r) { x^=r.x; y^=r.y; return *this; } \
	Vec2 &operator%=(const Vec2 &r) { x%=r.x; y%=r.y; return *this; } \


#define _DEFINE_VEC2_INT_FUNCTIONS(type, Vec2)                                                                        \
static inline Vec2 floor_div(const Vec2 &a, const Vec2 &b) { return Vec2(floor_div(a.x, b.x), floor_div(a.y, b.y)); } \
static inline Vec2 operator&(const Vec2 &l, const Vec2 &r) { return Vec2(l.x & r.x, l.y & r.y); }                     \
static inline Vec2 operator|(const Vec2 &l, const Vec2 &r) { return Vec2(l.x | r.x, l.y | r.y); }                     \
static inline Vec2 operator^(const Vec2 &l, const Vec2 &r) { return Vec2(l.x ^ r.x, l.y ^ r.y); }                     \
static inline Vec2 operator%(const Vec2 &l, const Vec2 &r) { return Vec2(l.x % r.x, l.y % r.y); }                     \


#define _DEFINE_VEC2_FLOAT_FUNCTIONS(type, Vec2)                                      \
static inline type length(const Vec2 &v) { return std::sqrt(length2(v)); }            \
static inline Vec2 normalize(const Vec2 &v) { return v / Vec2(length(v)); }           \
static inline type distance(const Vec2 &v1, const Vec2 &v2) { return length(v2-v1); } \


#define _DEFINE_VEC2(type, Vec2, ADDITIONAL_MEMBERS, ADDITIONAL_FUNCTIONS)                            \
struct Vec2 {                                                                                         \
	union {                                                                                           \
		struct {                                                                                      \
			type x, y;                                                                                \
		};                                                                                            \
		type data[2];                                                                                 \
	};                                                                                                \
                                                                                                      \
	Vec2() = default;                                                                                 \
	constexpr Vec2(type ax, type ay): x(ax), y(ay) {}                                                           \
	explicit constexpr Vec2(type v): x(v), y(v) {}                                                              \
                                                                                                      \
	Vec2 &operator+=(const Vec2 &r) { x+=r.x; y+=r.y; return *this; }                                 \
	Vec2 &operator-=(const Vec2 &r) { x-=r.x; y-=r.y; return *this; }                                 \
	Vec2 &operator*=(const Vec2 &r) { x*=r.x; y*=r.y; return *this; }                                 \
	Vec2 &operator/=(const Vec2 &r) { x/=r.x; y/=r.y; return *this; }                                 \
	ADDITIONAL_MEMBERS(type, Vec2)                                                                    \
                                                                                                      \
	type &operator[](int i) { return data[i]; }                                                       \
	type constexpr operator[](int i) const { return data[i]; }                                                  \
};                                                                                                    \
                                                                                                      \
static inline constexpr Vec2 Vec2##_X(type v = 1) { return {v, 0}; }                                            \
static inline constexpr Vec2 Vec2##_Y(type v = 1) { return {0, v}; }                                            \
                                                                                                      \
static inline constexpr bool operator==(const Vec2 &l, const Vec2 &r) { return l.x == r.x && l.y == r.y; }      \
static inline constexpr bool operator!=(const Vec2 &l, const Vec2 &r) { return l.x != r.x || l.y != r.y; }      \
static inline constexpr bool operator<(const Vec2 &l, const Vec2 &r)  { return l.x < r.x && l.y < r.y; }        \
static inline constexpr bool operator>(const Vec2 &l, const Vec2 &r)  { return l.x > r.x && l.y > r.y; }        \
static inline constexpr bool operator<=(const Vec2 &l, const Vec2 &r) { return l.x <= r.x && l.y <= r.y; }      \
static inline constexpr bool operator>=(const Vec2 &l, const Vec2 &r) { return l.x >= r.x && l.y >= r.y; }      \
                                                                                                      \
static inline constexpr Vec2 operator-(const Vec2 &v) { return Vec2(-v.x, -v.y); }                              \
static inline constexpr Vec2 operator+(const Vec2 &l, const Vec2 &r) { return Vec2(l.x + r.x, l.y + r.y); }     \
static inline constexpr Vec2 operator-(const Vec2 &l, const Vec2 &r) { return Vec2(l.x - r.x, l.y - r.y); }     \
static inline constexpr Vec2 operator*(const Vec2 &l, const Vec2 &r) { return Vec2(l.x * r.x, l.y * r.y); }     \
static inline constexpr Vec2 operator/(const Vec2 &l, const Vec2 &r) { return Vec2(l.x / r.x, l.y / r.y); }     \
                                                                                                      \
static inline constexpr type area(const Vec2 &v) { return v.x * v.y; }                                          \
static inline constexpr type length2(const Vec2 &v) { return v.x*v.x + v.y*v.y; }                               \
static inline constexpr type dot(const Vec2 &v1, const Vec2 &v2) { return v1.x*v2.x + v1.y*v2.y; }              \
static inline constexpr type distance2(const Vec2 &v1, const Vec2 &v2) { return length2(v2-v1); }               \
static inline Vec2 min(const Vec2 &v1, const Vec2 &v2) { return {min(v1.x, v2.x), min(v1.y, v2.y)}; } \
static inline Vec2 max(const Vec2 &v1, const Vec2 &v2) { return {max(v1.x, v2.x), max(v1.y, v2.y)}; } \
ADDITIONAL_FUNCTIONS(type, Vec2)                                                                      \


_DEFINE_VEC2(float,    Vec2f,  _DEFINE_VEC2_NO_MEMBERS,  _DEFINE_VEC2_FLOAT_FUNCTIONS)
_DEFINE_VEC2(double,   Vec2d,  _DEFINE_VEC2_NO_MEMBERS,  _DEFINE_VEC2_FLOAT_FUNCTIONS)
_DEFINE_VEC2(int32_t,  Vec2i,  _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)
_DEFINE_VEC2(int16_t,  Vec2s,  _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)
_DEFINE_VEC2(int8_t,   Vec2b,  _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)

_DEFINE_VEC2(uint16_t, Vec2us, _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)
_DEFINE_VEC2(uint8_t,  Vec2ub, _DEFINE_VEC2_INT_MEMBERS, _DEFINE_VEC2_INT_FUNCTIONS)

static inline constexpr Vec2f ToVec2f(const Vec2i &v) { return Vec2f(v.x, v.y); }
static inline constexpr Vec2i ToVec2i(const Vec2f &v) { return Vec2i(v.x, v.y); }
static inline constexpr Vec2i ToVec2i(const Vec2s &v) { return Vec2i(v.x, v.y); }
static inline constexpr Vec2s ToVec2s(const Vec2i &v) { return Vec2s(v.x, v.y); }

static inline int chebyshev_distance(const Vec2i &a, const Vec2i &b)
{
	const Vec2i diff = a - b;
	return max(std::abs(diff.x), std::abs(diff.y));
}

//------------------------------------------------------------------------------
// Vec3
//------------------------------------------------------------------------------

#define _DEFINE_VEC3_NO_FUNCTIONS(type, Vec3)
#define _DEFINE_VEC3_NO_MEMBERS(type, Vec3)


#define _DEFINE_VEC3_INT_MEMBERS(type, Vec3)                                  \
	Vec3 &operator&=(const Vec3 &r) { x&=r.x; y&=r.y; z&=r.z; return *this; } \
	Vec3 &operator|=(const Vec3 &r) { x|=r.x; y|=r.y; z|=r.z; return *this; } \
	Vec3 &operator^=(const Vec3 &r) { x^=r.x; y^=r.y; z^=r.z; return *this; } \
	Vec3 &operator%=(const Vec3 &r) { x%=r.x; y%=r.y; z%=r.z; return *this; } \


#define _DEFINE_VEC3_INT_FUNCTIONS(type, Vec3)                                                                                             \
static inline Vec3 floor_div(const Vec3 &a, const Vec3 &b) { return Vec3(floor_div(a.x, b.x), floor_div(a.y, b.y), floor_div(a.z, b.z)); } \
static inline constexpr Vec3 operator^(const Vec3 &l, const Vec3 &r) { return Vec3(l.x ^ r.x, l.y ^ r.y, l.z ^ r.z); }                               \
static inline constexpr Vec3 operator%(const Vec3 &l, const Vec3 &r) { return Vec3(l.x % r.x, l.y % r.y, l.z % r.z); }                               \
static inline constexpr Vec3 operator&(const Vec3 &l, const Vec3 &r) { return Vec3(l.x & r.x, l.y & r.y, l.z & r.z); }                               \
static inline constexpr Vec3 operator|(const Vec3 &l, const Vec3 &r) { return Vec3(l.x | r.x, l.y | r.y, l.z | r.z); }                               \
static inline constexpr Vec3 operator~(const Vec3 &v) { return Vec3(~v.x, ~v.y, ~v.z); }                                                             \


#define _DEFINE_VEC3_FLOAT_FUNCTIONS(type, Vec3)                                                                                          \
static inline Vec3 abs(const Vec3 &v) { return Vec3(std::abs(v.x), std::abs(v.y), std::abs(v.z)); }                                       \
static inline type length(const Vec3 &v) { return std::sqrt(length2(v)); }                                                                \
static inline Vec3 normalize(const Vec3 &v) { return v / Vec3(length(v)); }                                                               \
static inline bool is_nan(const Vec3 &v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z); }                                \
static inline type distance(const Vec3 &v1, const Vec3 &v2) { return length(v2-v1); }                                                     \
static inline constexpr Vec3 lerp(const Vec3 &a, const Vec3 &b, float v) { return a * Vec3(1 - v) + b * Vec3(v); }                                  \
static inline Vec3 mod(const Vec3 &a, const Vec3 &b) { return Vec3(std::fmod(a.x, b.x), std::fmod(a.y, b.y), std::fmod(a.z, b.z)); }      \
static inline Vec3 pow(const Vec3 &v1, const Vec3 &v2) { return Vec3(std::pow(v1.x, v2.x), std::pow(v1.y, v2.y), std::pow(v1.z, v2.z)); } \


#define _DEFINE_VEC3(type, Vec3, Vec2, ADDITIONAL_MEMBERS, ADDITIONAL_FUNCTIONS)                                                                           \
struct Vec3 {                                                                                                                                              \
	union {                                                                                                                                                \
		struct {                                                                                                                                           \
			type x, y, z;                                                                                                                                  \
		};                                                                                                                                                 \
		type data[3];                                                                                                                                      \
	};                                                                                                                                                     \
                                                                                                                                                           \
	Vec3() = default;                                                                                                                                      \
	constexpr Vec3(type ax, type ay, type az): x(ax), y(ay), z(az) {}                                                                                                \
	explicit constexpr Vec3(type v): x(v), y(v), z(v) {}                                                                                                             \
                                                                                                                                                           \
	Vec3 &operator+=(const Vec3 &r) { x+=r.x; y+=r.y; z+=r.z; return *this; }                                                                              \
	Vec3 &operator-=(const Vec3 &r) { x-=r.x; y-=r.y; z-=r.z; return *this; }                                                                              \
	Vec3 &operator*=(const Vec3 &r) { x*=r.x; y*=r.y; z*=r.z; return *this; }                                                                              \
	Vec3 &operator/=(const Vec3 &r) { x/=r.x; y/=r.y; z/=r.z; return *this; }                                                                              \
	ADDITIONAL_MEMBERS(type, Vec3)                                                                                                                         \
                                                                                                                                                           \
	type &operator[](int i) { return data[i]; }                                                                                                            \
	constexpr type operator[](int i) const { return data[i]; }                                                                                                       \
                                                                                                                                                           \
	constexpr Vec2 XY() const { return {x, y}; }                                                                                                                     \
	constexpr Vec2 XZ() const { return {x, z}; }                                                                                                                     \
	constexpr Vec2 YZ() const { return {y, z}; }                                                                                                                     \
};                                                                                                                                                         \
                                                                                                                                                           \
static inline constexpr Vec3 Vec3##_X(type v = 1) { return {v, 0, 0}; }                                                                                              \
static inline constexpr Vec3 Vec3##_Y(type v = 1) { return {0, v, 0}; }                                                                                              \
static inline constexpr Vec3 Vec3##_Z(type v = 1) { return {0, 0, v}; }                                                                                              \
static inline constexpr Vec3 Vec3##_XY(const Vec2 &v) { return {v.x, v.y, 0}; }                                                                                      \
static inline constexpr Vec3 Vec3##_XZ(const Vec2 &v) { return {v.x, 0, v.y}; }                                                                                      \
static inline constexpr Vec3 Vec3##_YZ(const Vec2 &v) { return {0, v.x, v.y}; }                                                                                      \
                                                                                                                                                           \
static inline constexpr bool operator==(const Vec3 &l, const Vec3 &r) { return l.x == r.x && l.y == r.y && l.z == r.z; }                                             \
static inline constexpr bool operator!=(const Vec3 &l, const Vec3 &r) { return l.x != r.x || l.y != r.y || l.z != r.z; }                                             \
static inline constexpr bool operator<(const Vec3 &l, const Vec3 &r)  { return l.x < r.x && l.y < r.y && l.z < r.z; }                                                \
static inline constexpr bool operator>(const Vec3 &l, const Vec3 &r)  { return l.x > r.x && l.y > r.y && l.z > r.z; }                                                \
static inline constexpr bool operator<=(const Vec3 &l, const Vec3 &r) { return l.x <= r.x && l.y <= r.y && l.z <= r.z; }                                             \
static inline constexpr bool operator>=(const Vec3 &l, const Vec3 &r) { return l.x >= r.x && l.y >= r.y && l.z >= r.z; }                                             \
static inline constexpr Vec3 operator+(const Vec3 &l, const Vec3 &r) { return Vec3(l.x + r.x, l.y + r.y, l.z + r.z); }                                               \
static inline constexpr Vec3 operator-(const Vec3 &l, const Vec3 &r) { return Vec3(l.x - r.x, l.y - r.y, l.z - r.z); }                                               \
static inline constexpr Vec3 operator*(const Vec3 &l, const Vec3 &r) { return Vec3(l.x * r.x, l.y * r.y, l.z * r.z); }                                               \
static inline constexpr Vec3 operator/(const Vec3 &l, const Vec3 &r) { return Vec3(l.x / r.x, l.y / r.y, l.z / r.z); }                                               \
static inline constexpr Vec3 operator-(const Vec3 &v) { return Vec3(-v.x, -v.y, -v.z); }                                                                             \
                                                                                                                                                           \
static inline constexpr type length2(const Vec3 &v) { return v.x*v.x + v.y*v.y + v.z*v.z; }                                                                          \
static inline constexpr type dot(const Vec3 &v1, const Vec3 &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }                                                       \
static inline constexpr type volume(const Vec3 &v) { return v.x * v.y * v.z; }                                                                                       \
static inline constexpr Vec3 cross(const Vec3 &v1, const Vec3 &v2) { return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); } \
static inline constexpr type distance2(const Vec3 &v1, const Vec3 &v2) { return length2(v2-v1); }                                                                    \
static inline Vec3 min(const Vec3 &v1, const Vec3 &v2) { return {min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z)}; }                                     \
static inline Vec3 max(const Vec3 &v1, const Vec3 &v2) { return {max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z)}; }                                     \
ADDITIONAL_FUNCTIONS(type, Vec3)                                                                                                                           \


_DEFINE_VEC3(float,   Vec3f, Vec2f, _DEFINE_VEC3_NO_MEMBERS,  _DEFINE_VEC3_FLOAT_FUNCTIONS)
_DEFINE_VEC3(double,  Vec3d, Vec2d, _DEFINE_VEC3_NO_MEMBERS,  _DEFINE_VEC3_FLOAT_FUNCTIONS)
_DEFINE_VEC3(int32_t, Vec3i, Vec2i, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)
_DEFINE_VEC3(int16_t, Vec3s, Vec2s, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)
_DEFINE_VEC3(int8_t,  Vec3b, Vec2b, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)

_DEFINE_VEC3(uint16_t, Vec3us, Vec2us, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)
_DEFINE_VEC3(uint8_t,  Vec3ub, Vec2ub, _DEFINE_VEC3_INT_MEMBERS, _DEFINE_VEC3_INT_FUNCTIONS)


static inline constexpr Vec3f ToVec3f(const Vec3i &v) { return Vec3f(v.x, v.y, v.z); }
static inline constexpr Vec3f ToVec3f(const Vec3d &v) { return Vec3f(v.x, v.y, v.z); }
static inline constexpr Vec3f ToVec3f(const Vec3ub &v) { return Vec3f(v.x, v.y, v.z); }
static inline constexpr Vec3f ToVec3f(const Vec3us &v) { return Vec3f(v.x, v.y, v.z); }
static inline constexpr Vec3d ToVec3d(const Vec3i &v) { return Vec3d(v.x, v.y, v.z); }
static inline constexpr Vec3d ToVec3d(const Vec3f &v) { return Vec3d(v.x, v.y, v.z); }
static inline constexpr Vec3i ToVec3i(const Vec3f &v) { return Vec3i(v.x, v.y, v.z); }
static inline constexpr Vec3i ToVec3i(const Vec3d &v) { return Vec3i(v.x, v.y, v.z); }
static inline constexpr Vec3i ToVec3i(const Vec3ub &v) { return Vec3i(v.x, v.y, v.z); }
static inline constexpr Vec3ub ToVec3ub(const Vec3i &v) { return Vec3ub(v.x, v.y, v.z); }
static inline constexpr Vec3us ToVec3us(const Vec3i &v) { return Vec3us(v.x, v.y, v.z); }

static inline Vec3i floor(const Vec3f &v) { return Vec3i(std::floor(v.x), std::floor(v.y), std::floor(v.z)); }
static inline Vec3i floor(const Vec3d &v) { return Vec3i(std::floor(v.x), std::floor(v.y), std::floor(v.z)); }

static inline constexpr bool axes_equal(const Vec3i &a, const Vec3i &b, const Vec2i &axes)
{
	return a[axes[0]] == b[axes[0]] && a[axes[1]] == b[axes[1]];
}

static inline constexpr bool aabb_aabb_intersection(const Vec3i &amin, const Vec3i &amax,
	const Vec3i &bmin, const Vec3i &bmax)
{
	return !(
		amax.x < bmin.x ||
		amax.y < bmin.y ||
		amax.z < bmin.z ||
		amin.x > bmax.x ||
		amin.y > bmax.y ||
		amin.z > bmax.z
	);
}

static inline int chebyshev_distance(const Vec3i &a, const Vec3i &b)
{
	const Vec3i diff = a - b;
	return max3(std::abs(diff.x), std::abs(diff.y), std::abs(diff.z));
}

//------------------------------------------------------------------------------
// Vec4
//------------------------------------------------------------------------------

struct Vec4f {
	union {
		struct {
			float x, y, z, w;
		};
		float data[4];
	};

	Vec4f() = default;
	constexpr Vec4f(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}
	explicit constexpr Vec4f(float v): x(v), y(v), z(v), w(v) {}

	Vec4f &operator+=(const Vec4f &r) { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
	Vec4f &operator-=(const Vec4f &r) { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
	Vec4f &operator*=(const Vec4f &r) { x*=r.x; y*=r.y; z*=r.z; w*=r.w; return *this; }
	Vec4f &operator/=(const Vec4f &r) { x/=r.x; y/=r.y; z/=r.z; w/=r.w; return *this; }

	float &operator[](int i) { return data[i]; }
	constexpr float operator[](int i) const { return data[i]; }
};

static inline constexpr Vec4f operator+(const Vec4f &l, const Vec4f &r) { return {l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w}; }
static inline constexpr Vec4f operator-(const Vec4f &l, const Vec4f &r) { return {l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w}; }
static inline constexpr Vec4f operator*(const Vec4f &l, const Vec4f &r) { return {l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w}; }
static inline constexpr Vec4f operator/(const Vec4f &l, const Vec4f &r) { return {l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w}; }
static inline constexpr bool operator==(const Vec4f &l, const Vec4f &r) { return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w; }
static inline constexpr bool operator!=(const Vec4f &l, const Vec4f &r) { return l.x != r.x || l.y != r.y || l.z != r.z || l.w != r.w; }
static inline constexpr bool operator<(const Vec4f &l, const Vec4f &r)  { return l.x < r.x && l.y < r.y && l.z < r.z && l.w < r.w; }
static inline constexpr bool operator>(const Vec4f &l, const Vec4f &r)  { return l.x > r.x && l.y > r.y && l.z > r.z && l.w > r.w; }
static inline constexpr bool operator<=(const Vec4f &l, const Vec4f &r) { return l.x <= r.x && l.y <= r.y && l.z <= r.z && l.w <= r.w; }
static inline constexpr bool operator>=(const Vec4f &l, const Vec4f &r) { return l.x >= r.x && l.y >= r.y && l.z >= r.z && l.w >= r.w; }

static inline constexpr float dot(const Vec4f &v1, const Vec4f &v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w; }

static inline constexpr Vec3f ToVec3f(const Vec4f &v) { return Vec3f(v.x, v.y, v.z); }
static inline constexpr Vec4f ToVec4f(const Vec3f &v) { return Vec4f(v.x, v.y, v.z, 1); }

//------------------------------------------------------------------------------
// Vec4i
//------------------------------------------------------------------------------

struct Vec4i {
	union {
		struct {
			int x, y, z, w;
		};
		int data[4];
	};

	Vec4i() = default;
	constexpr Vec4i(int x, int y, int z, int w): x(x), y(y), z(z), w(w) {}
	explicit constexpr Vec4i(int v): x(v), y(v), z(v), w(v) {}

	Vec4i &operator+=(const Vec4i &r) { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
	Vec4i &operator-=(const Vec4i &r) { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
	Vec4i &operator*=(const Vec4i &r) { x*=r.x; y*=r.y; z*=r.z; w*=r.w; return *this; }
	Vec4i &operator/=(const Vec4i &r) { x/=r.x; y/=r.y; z/=r.z; w/=r.w; return *this; }

	int &operator[](int i) { return data[i]; }
	int constexpr operator[](int i) const { return data[i]; }
};

static inline constexpr Vec4i operator+(const Vec4i &l, const Vec4i &r) { return {l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w}; }
static inline constexpr Vec4i operator-(const Vec4i &l, const Vec4i &r) { return {l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w}; }
static inline constexpr Vec4i operator*(const Vec4i &l, const Vec4i &r) { return {l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w}; }
static inline constexpr Vec4i operator/(const Vec4i &l, const Vec4i &r) { return {l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w}; }
static inline constexpr bool operator==(const Vec4i &l, const Vec4i &r) { return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w; }
static inline constexpr bool operator!=(const Vec4i &l, const Vec4i &r) { return l.x != r.x || l.y != r.y || l.z != r.z || l.w != r.w; }
static inline constexpr bool operator<(const Vec4i &l, const Vec4i &r)  { return l.x < r.x && l.y < r.y && l.z < r.z && l.w < r.w; }
static inline constexpr bool operator>(const Vec4i &l, const Vec4i &r)  { return l.x > r.x && l.y > r.y && l.z > r.z && l.w > r.w; }
static inline constexpr bool operator<=(const Vec4i &l, const Vec4i &r) { return l.x <= r.x && l.y <= r.y && l.z <= r.z && l.w <= r.w; }
static inline constexpr bool operator>=(const Vec4i &l, const Vec4i &r) { return l.x >= r.x && l.y >= r.y && l.z >= r.z && l.w >= r.w; }

static inline constexpr Vec4f ToVec4(const Vec4i &v) { return Vec4f(v.x, v.y, v.z, v.w); }
static inline constexpr Vec4i ToVec4i(const Vec4f &v) { return Vec4i(v.x, v.y, v.z, v.w); }

//------------------------------------------------------------------------------
// Vec4us
//------------------------------------------------------------------------------

struct Vec4us {
	union {
		struct {
			uint16_t x, y, z, w;
		};
		uint16_t data[4];
	};

	Vec4us() = default;
	constexpr Vec4us(uint16_t x, uint16_t y, uint16_t z, uint16_t w): x(x), y(y), z(z), w(w) {}
	explicit constexpr Vec4us(int v): x(v), y(v), z(v), w(v) {}

	Vec4us &operator+=(const Vec4us &r) { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
	Vec4us &operator-=(const Vec4us &r) { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
	Vec4us &operator*=(const Vec4us &r) { x*=r.x; y*=r.y; z*=r.z; w*=r.w; return *this; }
	Vec4us &operator/=(const Vec4us &r) { x/=r.x; y/=r.y; z/=r.z; w/=r.w; return *this; }

	uint16_t &operator[](int i) { return data[i]; }
	uint16_t constexpr operator[](int i) const { return data[i]; }
};

static inline constexpr Vec4us operator+(const Vec4us &l, const Vec4us &r) { return Vec4us(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w); }
static inline constexpr Vec4us operator-(const Vec4us &l, const Vec4us &r) { return Vec4us(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w); }
static inline constexpr Vec4us operator*(const Vec4us &l, const Vec4us &r) { return Vec4us(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w); }
static inline constexpr Vec4us operator/(const Vec4us &l, const Vec4us &r) { return Vec4us(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w); }

static inline constexpr Vec3f ToVec3f(const Vec4us &v) { return Vec3f(v.x, v.y, v.z); }

//------------------------------------------------------------------------------
// Vec4ub
//------------------------------------------------------------------------------

struct Vec4ub {
	union {
		struct {
			uint8_t x, y, z, w;
		};
		uint8_t data[4];
	};

	Vec4ub() = default;
	constexpr Vec4ub(uint8_t x, uint8_t y, uint8_t z, uint8_t w): x(x), y(y), z(z), w(w) {}
	explicit constexpr Vec4ub(int v): x(v), y(v), z(v), w(v) {}

	Vec4ub &operator+=(const Vec4ub &r) { x+=r.x; y+=r.y; z+=r.z; w+=r.w; return *this; }
	Vec4ub &operator-=(const Vec4ub &r) { x-=r.x; y-=r.y; z-=r.z; w-=r.w; return *this; }
	Vec4ub &operator*=(const Vec4ub &r) { x*=r.x; y*=r.y; z*=r.z; w*=r.w; return *this; }
	Vec4ub &operator/=(const Vec4ub &r) { x/=r.x; y/=r.y; z/=r.z; w/=r.w; return *this; }

	uint8_t &operator[](int i) { return data[i]; }
	uint8_t constexpr operator[](int i) const { return data[i]; }
};

static inline constexpr Vec4ub operator+(const Vec4ub &l, const Vec4ub &r) { return Vec4ub(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w); }
static inline constexpr Vec4ub operator-(const Vec4ub &l, const Vec4ub &r) { return Vec4ub(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w); }
static inline constexpr Vec4ub operator*(const Vec4ub &l, const Vec4ub &r) { return Vec4ub(l.x * r.x, l.y * r.y, l.z * r.z, l.w * r.w); }
static inline constexpr Vec4ub operator/(const Vec4ub &l, const Vec4ub &r) { return Vec4ub(l.x / r.x, l.y / r.y, l.z / r.z, l.w / r.w); }

static inline constexpr Vec4ub ToVec4ub(const Vec4i &v) { return Vec4ub(v.x, v.y, v.z, v.w); }
static inline constexpr Vec4ub ToVec4ub(const Vec4f &v) { return Vec4ub(v.x * 255.0f, v.y * 255.0f, v.z * 255.0f, v.w * 255.0f); }

//------------------------------------------------------------------------------
// Macro Utils
//------------------------------------------------------------------------------

#define VEC2(v) (v).x, (v).y
#define VEC3(v) (v).x, (v).y, (v).z
#define VEC4(v) (v).x, (v).y, (v).z, (v).w
