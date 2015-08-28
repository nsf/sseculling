#pragma once

#include <utility>

template <typename T> class Func;

template <typename R, typename ...Args>
class Func<R (Args...)> {
	static R _invoke_func(void *data, Args &&...args)
	{
		auto fp = reinterpret_cast<R (*)(Args...)>(data);
		return (*fp)(std::forward<Args>(args)...);
	}

	template <typename T>
	static R _invoke_obj(void *data, Args &&...args)
	{
		auto obj = static_cast<T*>(data);
		return (*obj)(std::forward<Args>(args)...);
	}

	template <typename T>
	static R _invoke_const_obj(void *data, Args &&...args)
	{
		auto obj = static_cast<const T*>(data);
		return (*obj)(std::forward<Args>(args)...);
	}

	R (*m_invoker)(void*, Args&&...) = nullptr;
	void *m_data = nullptr;

public:
	Func() = default;
	Func(const Func &r) = default;
	Func(Func &&r) = default;

	// We need this little guy here, because otherwise template ctor
	// func(T &obj) will be considered as a better overload match
	// in some cases

	// Func(Func &r) = default;

	template <typename T>
	Func(T &obj): m_invoker(_invoke_obj<T>),
		m_data(static_cast<void*>(&obj))
	{
	}
	template <typename T>
	Func(const T &obj): m_invoker(_invoke_const_obj<T>),
		m_data(static_cast<void*>(const_cast<T*>(&obj)))
	{
	}
	Func(R (*fp)(Args...)): m_invoker(_invoke_func),
		m_data(reinterpret_cast<void*>(fp))
	{
	}

	Func &operator=(const Func&) = default;
	Func &operator=(Func&&) = default;
	R operator()(Args ...args) const
	{
		return (*m_invoker)(m_data, std::forward<Args>(args)...);
	}

	explicit operator bool() const { return m_data != nullptr; }
};
