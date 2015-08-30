#pragma once

#include "Core/Memory.h"
#include "Core/Utils.h"

template <typename T>
struct DefaultDelete {
	static void invoke(T *ptr) {
		delete ptr;
	}
};

template <typename T>
struct DefaultDelete<T[]> {
	static void invoke(T *ptr) {
		delete [] ptr;
	}
};

template <typename T, typename D = DefaultDelete<T>>
class UniquePtr {
	T *ptr;

public:
	constexpr UniquePtr(): ptr(nullptr) {}
	constexpr UniquePtr(std::nullptr_t): ptr(nullptr) {}
	explicit UniquePtr(T *ptr): ptr(ptr) {}
	UniquePtr(const UniquePtr&) = delete;
	UniquePtr(UniquePtr &&r): ptr(r.release()) {}
	~UniquePtr() { if (ptr) D::invoke(ptr); }

	UniquePtr &operator=(const UniquePtr&) = delete;
	UniquePtr &operator=(UniquePtr &&r)
	{
		if (ptr) D::invoke(ptr);
		ptr = r.release();
		return *this;
	}

	UniquePtr &operator=(std::nullptr_t)
	{
		if (ptr) D::invoke(ptr);
		ptr = nullptr;
		return *this;
	}

	T &operator*() const
	{
		NG_ASSERT(ptr != nullptr);
		return *ptr;
	}

	T* operator->() const { NG_ASSERT(ptr != nullptr); return ptr; }
	T* get() const { return ptr; }
	explicit operator bool() const { return ptr != nullptr; }

	T *release() { T *ret = ptr; ptr = nullptr; return ret; }
	void reset(T *p = nullptr)
	{
		if (ptr) D::invoke(ptr);
		ptr = p;
	}
};

template <typename T, typename D>
class UniquePtr<T[], D> : public UniquePtr<T, D> {
public:
	constexpr UniquePtr() = default;
	constexpr UniquePtr(std::nullptr_t): UniquePtr<T, D>(nullptr) {}
	explicit UniquePtr(T *ptr): UniquePtr<T, D>(ptr) {}

	// #TODO:0 operator[]
};

template <typename T, typename D>
bool operator==(const UniquePtr<T, D> &lhs, const UniquePtr<T, D> &rhs)
{
	return lhs.get() == rhs.get();
}

template <typename T, typename D>
bool operator!=(const UniquePtr<T, D> &lhs, const UniquePtr<T, D> &rhs)
{
	return lhs.get() != rhs.get();
}

template <typename T, typename D>
bool operator==(const UniquePtr<T, D> &lhs, const T *rhs)
{
	return lhs.get() == rhs;
}

template <typename T, typename D>
bool operator!=(const T *lhs, const UniquePtr<T, D> &rhs)
{
	return lhs != rhs.get();
}

template <typename T, typename D>
bool operator==(const UniquePtr<T, D> &lhs, std::nullptr_t)
{
	return lhs.get() == nullptr;
}
template <typename T, typename D>
bool operator==(std::nullptr_t, const UniquePtr<T, D> &rhs)
{
	return nullptr == rhs.get();
}

template <typename T, typename D>
bool operator!=(const UniquePtr<T, D> &lhs, std::nullptr_t)
{
	return lhs.get() != nullptr;
}
template <typename T, typename D>
bool operator!=(std::nullptr_t, const UniquePtr<T, D> &rhs)
{
	return nullptr != rhs.get();
}

template <typename T, typename ...Args>
UniquePtr<T> make_unique(Args &&...args)
{
	return UniquePtr<T>(new (OrDie) T(std::forward<Args>(args)...));
}
