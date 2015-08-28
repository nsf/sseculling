#pragma once

#include <cstring>
#include <initializer_list>
#include <algorithm>
#include "Core/Utils.h"

#define _COMMON_SLICE_PART_CONST(T)                                         \
	const T *data;                                                          \
	int length;                                                             \
                                                                            \
	Slice() = default;                                                      \
	Slice(std::initializer_list<T> r): data(r.begin()), length(r.size()) {} \
	template <int N>                                                        \
	Slice(const T (&array)[N]): data(array), length(N) {}                   \
	Slice(const T *data, int len): data(data), length(len) {}               \
	Slice(const Slice<T> &r): data(r.data), length(r.length) {}             \
	explicit operator bool() const { return length != 0; }                  \
	int byte_length() const { return length * sizeof(T); }                  \
	const T &first() const { NG_ASSERT(length != 0); return data[0]; }      \
	const T &last() const { NG_ASSERT(length != 0); return data[length-1]; }\
	Slice<const T> sub() const                                              \
	{                                                                       \
		return {data, length};                                              \
	}                                                                       \
	Slice<const T> sub(int begin) const                                     \
	{                                                                       \
		NG_SLICE_BOUNDS_CHECK(begin, length);                               \
		return {data + begin, length - begin};                              \
	}                                                                       \
	Slice<const T> sub(int begin, int end) const                            \
	{                                                                       \
		NG_ASSERT(begin <= end);                                            \
		NG_SLICE_BOUNDS_CHECK(begin, length);                               \
		NG_SLICE_BOUNDS_CHECK(end, length);                                 \
		return {data + begin, end - begin};                                 \
	}                                                                       \
	const T &operator[](int idx) const                                      \
	{                                                                       \
		NG_IDX_BOUNDS_CHECK(idx, length);                                   \
		return data[idx];                                                   \
	}

template <typename T>
struct Slice {
	T *data;
	int length;

	Slice() = default;

	template <int N>
	Slice(T (&array)[N]): data(array), length(N) {}
	Slice(T *data, int length): data(data), length(length) {}
	explicit operator bool() const { return length != 0; }

	T &operator[](int idx)
	{
		NG_IDX_BOUNDS_CHECK(idx, length);
		return data[idx];
	}

	const T &operator[](int idx) const
	{
		NG_IDX_BOUNDS_CHECK(idx, length);
		return data[idx];
	}

	// for consistency with Vector, but feel free to use length directly
	int byte_length() const { return length * sizeof(T); }

	T &first() { NG_ASSERT(length != 0); return data[0]; }
	const T &first() const { NG_ASSERT(length != 0); return data[0]; }
	T &last() { NG_ASSERT(length != 0); return data[length-1]; }
	const T &last() const { NG_ASSERT(length != 0); return data[length-1]; }

	Slice sub()
	{
		return {data, length};
	}
	Slice sub(int begin)
	{
		NG_SLICE_BOUNDS_CHECK(begin, length);
		return {data + begin, length - begin};
	}
	Slice sub(int begin, int end)
	{
		NG_ASSERT(begin <= end);
		NG_SLICE_BOUNDS_CHECK(begin, length);
		NG_SLICE_BOUNDS_CHECK(end, length);
		return {data + begin, end - begin};
	}
	Slice<const T> sub() const
	{
		return {data, length};
	}
	Slice<const T> sub(int begin) const
	{
		NG_SLICE_BOUNDS_CHECK(begin, length);
		return {data + begin, length - begin};
	}
	Slice<const T> sub(int begin, int end) const
	{
		NG_ASSERT(begin <= end);
		NG_SLICE_BOUNDS_CHECK(begin, length);
		NG_SLICE_BOUNDS_CHECK(end, length);
		return {data + begin, end - begin};
	}
};

template <typename T>
struct Slice<const T> {
	_COMMON_SLICE_PART_CONST(T)
};

template <>
struct Slice<const char> {
	_COMMON_SLICE_PART_CONST(char)
	Slice(const char *str): data(str), length(str != nullptr ? strlen(str) : 0) {}
};

#undef _COMMON_SLICE_PART_CONST

template <typename T, typename U>
bool operator==(Slice<T> lhs, Slice<U> rhs)
{
	if (lhs.length != rhs.length)
		return false;
	for (int i = 0; i < lhs.length; i++) {
		if (!(lhs.data[i] == rhs.data[i]))
			return false;
	}
	return true;
}

template <typename T, typename U>
bool operator<(Slice<T> lhs, Slice<U> rhs)
{
	for (int i = 0; i < rhs.length; i++) {
		if (i == lhs.length) {
			// lhs.len() < rhs.len(), but the common part is ==
			return true;
		}
		if (lhs.data[i] < rhs.data[i]) {
			return true;
		}
		if (rhs.data[i] < lhs.data[i]) {
			return false;
		}
	}
	return false;
}

template <typename T, typename U>
bool operator!=(Slice<T> lhs, Slice<U> rhs)
{
	return !operator==(lhs, rhs);
}

template <typename T, typename U>
bool operator>=(Slice<T> lhs, Slice<U> rhs)
{
	return !operator<(lhs, rhs);
}

template <typename T, typename U>
bool operator<=(Slice<T> lhs, Slice<U> rhs)
{
	return !operator<(rhs, lhs);
}

template <typename T, typename U>
bool operator>(Slice<T> lhs, Slice<U> rhs)
{
	return operator<(rhs, lhs);
}

bool operator==(Slice<const char> lhs, Slice<const char> rhs);
bool operator<(Slice<const char> lhs, Slice<const char> rhs);
bool operator!=(Slice<const char> lhs, Slice<const char> rhs);
bool operator>=(Slice<const char> lhs, Slice<const char> rhs);
bool operator<=(Slice<const char> lhs, Slice<const char> rhs);
bool operator>(Slice<const char> lhs, Slice<const char> rhs);

template <typename T>
const T *begin(Slice<const T> s) { return s.data; }
template <typename T>
const T *end(Slice<const T> s) { return s.data + s.length; }
template <typename T>
T *begin(Slice<T> s) { return s.data; }
template <typename T>
T *end(Slice<T> s) { return s.data + s.length; }

template <typename T, typename U>
int copy(Slice<T> dst, Slice<U> src)
{
	const int n = std::min(dst.length, src.length);
	if (n == 0) {
		return 0;
	}

	const T *srcp = src.data;
	T *dstp = dst.data;
	if (srcp == dstp) {
		return n;
	}

	if (srcp < dstp) {
		for (int i = n-1; i >= 0; i--) {
			dstp[i] = srcp[i];
		}
	} else {
		for (int i = 0; i < n; i++) {
			dstp[i] = srcp[i];
		}
	}
	return n;
}

template <typename T, typename U>
Slice<T> slice_cast(Slice<U> s)
{
	const double ratio = (double)sizeof(U) / (double)sizeof(T);
	return Slice<T>((T*)s.data, s.length * ratio);
}

int compute_hash(Slice<const char> s);
int compute_hash(const char *s);
int compute_hash(int i);

template <typename T>
static inline int compute_hash(Slice<T> s)
{
	return compute_hash(slice_cast<const char>(s));
}

template <typename T>
void sort(Slice<T> s)
{
	std::sort(s.data, s.data + s.length);
}

template <typename T, typename F>
void sort(Slice<T> s, F &&f)
{
	std::sort(s.data, s.data + s.length, std::forward<F>(f));
}

template <typename T>
void stable_sort(Slice<T> s)
{
	std::stable_sort(s.data, s.data + s.length);
}

template <typename T, typename F>
void stable_sort(Slice<T> s, F &&f)
{
	std::stable_sort(s.data, s.data + s.length, std::forward<F>(f));
}

template <typename T>
void reverse(Slice<T> s)
{
	const int len1 = s.length - 1;
	const int mid = s.length / 2;
	for (int i = 0; i < mid; i++) {
		std::swap(s[i], s[len1-i]);
	}
}

template <typename T>
void fill(Slice<T> s, const T &v)
{
	for (int i = 0; i < s.length; i++)
		s.data[i] = v;
}

template <typename T, typename U>
int linear_find(Slice<T> s, const U &v)
{
	for (int i = 0; i < s.length; i++) {
		if (s.data[i] == v)
			return i;
	}
	return -1;
}

template <typename T, typename F>
int linear_find_if(Slice<T> s, F &&f)
{
	for (int i = 0; i < s.length; i++)
		if (f(s.data[i]))
			return i;
	return -1;
}

template <typename T, typename U>
int binary_find(Slice<T> s, const U &v)
{
	int imax = s.length-1;
	int imin = 0;

	while (imin < imax) {
		// believe it or not, nobody really cares about overflows here
		const int imid = (imin + imax) / 2;
		if (s.data[imid] < v)
			imin = imid+1;
		else
			imax = imid;
	}
	if (imax == imin && s.data[imin] == v)
		return imin;
	return -1;
}
