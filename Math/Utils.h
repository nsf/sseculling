#pragma once

//------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------

template <typename T>
static inline T lerp(const T &a, const T &b, float v)
{
	return a * (1 - v) + b * v;
}

template <typename T>
static inline T clamp(const T &value, const T &min, const T &max)
{
	if (value > max)
		return max;
	if (value < min)
		return min;
	return value;
}

template <typename T>
static inline const T &max(const T &v1, const T &v2)
{
	return (v1 > v2) ? v1 : v2;
}

// returns 0 if v1 > v2 or 1 otherwise
template <typename T>
static inline int max_i(const T &v1, const T &v2)
{
	return (v1 > v2) ? 0 : 1;
}

template <typename T>
static inline const T &min(const T &v1, const T &v2)
{
	return (v1 < v2) ? v1 : v2;
}

// returns 0 if v1 < v2 or 1 otherwise
template <typename T>
static inline int min_i(const T &v1, const T &v2)
{
	return (v1 < v2) ? 0 : 1;
}

template <typename T>
static inline const T &min3(const T &v1, const T &v2, const T &v3)
{
	return min(v1, min(v2, v3));
}

template <typename T>
static inline const T &max3(const T &v1, const T &v2, const T &v3)
{
	return max(v1, max(v2, v3));
}

template <typename T>
static inline int min3_i(const T &v1, const T &v2, const T &v3)
{
	const T *vs[] = {&v1, &v2, &v3};
	int min = 0;
	for (int i = 1; i < 3; i++) {
		if (*vs[i] < *vs[min])
			min = i;
	}
	return min;
}

template <typename T>
static inline int max3_i(const T &v1, const T &v2, const T &v3)
{
	const T *vs[] = {&v1, &v2, &v3};
	int max = 0;
	for (int i = 1; i < 3; i++) {
		if (*vs[i] > *vs[max])
			max = i;
	}
	return max;
}

static inline int next_power_of_2(int v)
{
	v -= 1;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}

static inline int floor_div(int a, int b)
{
	int q = a / b;
	int r = a % b;
	if (r != 0 && ((r < 0) != (b < 0))) q--;
	return q;
}

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

const float MATH_PI = 3.14159265359f;
const float MATH_2PI = MATH_PI * 2.0f;
const float MATH_HALF_PI = MATH_PI / 2.0f;
const float MATH_DEG_TO_RAD = MATH_PI / 180.0f;
const float MATH_RAD_TO_DEG = 180.0f / MATH_PI;
const float MATH_EPSILON = 1e-6f;
