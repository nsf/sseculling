#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "Core/Memory.h"
#include "Core/Utils.h"

void *xmalloc(int n)
{
	void *mem = malloc(n);
	if (!mem)
		die("nextgame: out of memory");
	return mem;
}

void xfree(void *ptr)
{
	free(ptr);
}

int xcopy(void *dst, const void *src, int n)
{
	memmove(dst, src, n);
	return n;
}

void xclear(void *dst, int n)
{
	memset(dst, 0, n);
}

void *operator new(size_t size, const OrDie_t &)
{
	void *mem = operator new(size, std::nothrow);
	if (!mem)
		die("nextgame: out of memory");
	return mem;
}

void *operator new[](size_t size, const OrDie_t &)
{
	void *mem = operator new[](size, std::nothrow);
	if (!mem)
		die("nextgame: out of memory");
	return mem;
}

void *DefaultAllocator::allocate_bytes(int n)
{
	return xmalloc(n);
}

void DefaultAllocator::free_bytes(void *mem)
{
	xfree(mem);
}

DefaultAllocator default_allocator;

AlignedAllocator::AlignedAllocator(int n): align_to(n)
{
}

void *AlignedAllocator::allocate_bytes(int n)
{
#ifdef _WIN32
	void *ptr = _aligned_malloc(n, align_to);
#else
	void *ptr;
	if (posix_memalign(&ptr, align_to, n) != 0)
		ptr = nullptr;
#endif
	if (ptr == nullptr)
		die("nextgame: out of memory (aligned: %d)", align_to);
	return ptr;
}

void AlignedAllocator::free_bytes(void *mem)
{
#ifdef _WIN32
	_aligned_free(mem);
#else
	free(mem);
#endif
}

AlignedAllocator sse_allocator(16);
