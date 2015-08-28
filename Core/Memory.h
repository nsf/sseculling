#pragma once

#include <new>
#include <utility>
#include <type_traits>
#include <cstddef>

void *xmalloc(int n);
void xfree(void *ptr);
int xcopy(void *dst, const void *src, int n);
void xclear(void *dst, int n);

struct OrDie_t {};
const OrDie_t OrDie = {};

void *operator new(size_t size, const OrDie_t&);
void *operator new[](size_t size, const OrDie_t&);

template <typename T>
T *allocate_memory(int n = 1)
{
	return (T*)xmalloc(sizeof(T) * n);
}

template <typename T>
T &allocate_memory(T *&ptr)
{
	ptr = (T*)xmalloc(sizeof(T));
	return *ptr;
}

template <typename T>
void free_memory(T *ptr)
{
	if (ptr) xfree(ptr);
}

template <typename T>
int copy_memory(T *dst, const T *src, int n = 1)
{
	return xcopy(dst, src, sizeof(T) * n);
}

template <typename T>
void clear_memory(T *dst, int n = 1)
{
	xclear(dst, sizeof(T)*n);
}

struct Allocator {
	virtual void *allocate_bytes(int n) = 0;
	virtual void free_bytes(void *mem) = 0;

	template <typename T>
	T *allocate_memory(int n = 1)
	{
		return (T*)allocate_bytes(sizeof(T) * n);
	}

	template <typename T>
	T &allocate_memory(T *&ptr)
	{
		ptr = (T*)allocate_bytes(sizeof(T));
		return *ptr;
	}

	template <typename T>
	void free_memory(T *ptr)
	{
		if (ptr) free_bytes(ptr);
	}
};

struct DefaultAllocator : Allocator {
	void *allocate_bytes(int n) override;
	void free_bytes(void *mem) override;
};

extern DefaultAllocator default_allocator;

struct AlignedAllocator : Allocator {
	int align_to;

	AlignedAllocator(int n);
	void *allocate_bytes(int n) override;
	void free_bytes(void *mem) override;
};

// aligned to 16 bytes
extern AlignedAllocator sse_allocator;
