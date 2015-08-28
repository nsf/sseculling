#pragma once

#include "Core/Utils.h"
#include "Core/Memory.h"
#include "Core/Slice.h"

template <typename T>
struct Vector {
	T *m_data = nullptr;
	int m_len = 0;
	int m_cap = 0;
	Allocator *m_allocator = &default_allocator;

	int _new_size(int requested) const
	{
		int newcap = m_cap * 2;
		return newcap < requested ? requested : newcap;
	}

	void _ensure_capacity(int n)
	{
		if (m_len + n > m_cap)
			reserve(_new_size(m_len + n));
	}

	// expects: idx < _len, idx >= 0, offset > 0
	void _move_forward(int idx, int offset)
	{
		const int last = m_len-1;
		int src = last;
		int dst = last+offset;
		while (src >= idx) {
			new (&m_data[dst]) T(std::move(m_data[src]));
			m_data[src].~T();
			src--;
			dst--;
		}
	}

	// expects: idx < _len, idx >= 0, offset < 0
	void _move_backward(int idx, int offset)
	{
		int src = idx;
		int dst = idx+offset;
		while (src < m_len) {
			new (&m_data[dst]) T(std::move(m_data[src]));
			m_data[src].~T();
			src++;
			dst++;
		}
	}

	void _self_insert(int idx, Slice<const T> s)
	{
		int sidx = s.data - m_data;
		_ensure_capacity(s.length);

		// restore the slice after possible realloc
		s = Slice<const T>(m_data + sidx, s.length);

		// shorcut case, append
		if (idx == m_len) {
			for (int i = 0; i < s.length; i++)
				new (m_data + idx + i) T(s.data[i]);
			m_len += s.length;
			return;
		}

		// move tail further towards the end so that there is a free space for
		// data insertion
		_move_forward(idx, s.length);
		m_len += s.length;

		// adjust slice so that it points to the right data and if we're
		// splitting the slice, insert first half of it right away
		if (idx <= sidx) {
			s = Slice<const T>(s.data + s.length, s.length);
		} else {
			const int lhslen = idx - sidx;
			copy_memory(m_data + idx, m_data + sidx, lhslen);
			for (int i = 0; i < lhslen; i++)
				new (m_data + idx + i) T(m_data[sidx+i]);
			idx += lhslen;
			s = Slice<const T>(s.data + s.length + lhslen, s.length - lhslen);
		}
		for (int i = 0; i < s.length; i++)
			new (m_data + idx + i) T(s.data[i]);
	}

	void _nullify()
	{
		m_data = nullptr;
		m_len = 0;
		m_cap = 0;
	}

	Vector() = default;

	explicit Vector(Allocator *allocator): m_allocator(allocator) {}

	explicit Vector(int n): m_len(n), m_cap(n)
	{
		NG_ASSERT(n >= 0);
		if (m_len == 0)
			return;
		m_data = m_allocator->allocate_memory<T>(m_len);
		for (int i = 0; i < m_len; i++)
			new (m_data + i) T;
	}

	Vector(int n, const T &elem): m_len(n), m_cap(n)
	{
		NG_ASSERT(n >= 0);
		if (m_len == 0)
			return;
		m_data = m_allocator->allocate_memory<T>(m_len);
		for (int i = 0; i < m_len; i++)
			new (m_data + i) T(elem);
	}

	Vector(Slice<const T> s): m_len(s.length), m_cap(s.length)
	{
		if (m_len == 0)
			return;
		m_data = m_allocator->allocate_memory<T>(m_len);
		for (int i = 0; i < m_len; i++)
			new (m_data + i) T(s.data[i]);
	}

	Vector(Slice<T> s): Vector(Slice<const T>(s))
	{
	}

	Vector(std::initializer_list<T> r): Vector(Slice<const T>(r))
	{
	}

	Vector(const Vector &r): Vector(Slice<const T>(r))
	{
	}

	Vector(Vector &&r): m_data(r.m_data), m_len(r.m_len), m_cap(r.m_cap)
	{
		if (m_allocator != r.m_allocator)
			die("Vector: moving is only allowed between vectors with the same allocator");
		r._nullify();
	}

	Vector &operator=(Slice<const T> r)
	{
		if (m_data == r.data && m_len == r.length) {
			// self copy shortcut (a = a)
			return *this;
		}
		if (m_cap < r.length) {
			// slice is bigger than we are, realloc needed, also it
			// means slice cannot point to ourselves and it is save
			// to destroy ourselves
			for (int i = 0; i < m_len; i++)
				m_data[i].~T();
			m_allocator->free_memory(m_data);
			m_cap = m_len = r.length;
			m_data = m_allocator->allocate_memory<T>(m_len);
			for (int i = 0; i < m_len; i++)
				new (m_data + i) T(r.data[i]);
		} else {
			// slice can be a subset of ourselves
			int i = copy(sub(), r);
			for (; i < m_len; i++) {
				// destroy the rest if any
				m_data[i].~T();
			}
			for (; i < r.length; i++) {
				// construct the new if any
				new (m_data + i) T(r.data[i]);
			}
			m_len = r.length;
		}
		return *this;
	}

	Vector &operator=(Slice<T> r)
	{
		return operator=(Slice<const T>(r));
	}

	Vector &operator=(const Vector &r)
	{
		return operator=(Slice<const T>(r));
	}

	Vector &operator=(Vector &&r)
	{
		if (m_allocator != r.m_allocator)
			die("Vector: moving is only allowed between vectors with the same allocator");
		for (int i = 0; i < m_len; i++)
			m_data[i].~T();
		m_allocator->free_memory(m_data);

		m_data = r.m_data;
		m_len = r.m_len;
		m_cap = r.m_cap;
		r._nullify();
		return *this;
	}

	Vector &operator=(std::initializer_list<T> r)
	{
		return operator=(Slice<const T>(r));
	}

	~Vector()
	{
		for (int i = 0; i < m_len; i++)
			m_data[i].~T();
		m_allocator->free_memory(m_data);
	}

	int length() const { return m_len; }
	int byte_length() const { return m_len * sizeof(T); }
	int capacity() const { return m_cap; }
	T *data() { return m_data; }
	const T *data() const { return m_data; }

	void clear()
	{
		for (int i = 0; i < m_len; i++)
			m_data[i].~T();
		m_len = 0;
	}

	void reserve(int n)
	{
		if (m_cap >= n)
			return;

		T *old_data = m_data;
		m_cap = n;
		m_data = m_allocator->allocate_memory<T>(m_cap);
		for (int i = 0; i < m_len; i++) {
			new (m_data + i) T(std::move(old_data[i]));
			old_data[i].~T();
		}
		m_allocator->free_memory(old_data);
	}

	void shrink()
	{
		if (m_cap == m_len)
			return;

		T *old_data = m_data;
		m_cap = m_len;
		if (m_len > 0) {
			m_data = m_allocator->allocate_memory<T>(m_len);
			for (int i = 0; i < m_len; i++) {
				new (m_data + i) T(std::move(old_data[i]));
				old_data[i].~T();
			}
		} else {
			m_data = nullptr;
		}
		m_allocator->free_memory(old_data);
	}

	void resize(int n)
	{
		NG_ASSERT(n >= 0);

		if (m_len == n)
			return;

		if (m_len > n) {
			for (int i = n; i < m_len; i++)
				m_data[i].~T();
			m_len = n;
			return;
		}

		reserve(n);
		for (int i = m_len; i < n; i++)
			new (m_data + i) T;
		m_len = n;
	}

	void resize(int n, const T &elem)
	{
		NG_ASSERT(n >= 0);

		if (m_len == n)
			return;

		if (m_len > n) {
			for (int i = n; i < m_len; i++)
				m_data[i].~T();
			m_len = n;
			return;
		}

		reserve(n);
		for (int i = m_len; i < n; i++)
			new (m_data + i) T(elem);
		m_len = n;
	}

	void quick_remove(int idx)
	{
		NG_IDX_BOUNDS_CHECK(idx, m_len);
		if (idx != m_len-1)
			std::swap(m_data[idx], m_data[m_len-1]);
		m_data[--m_len].~T();
	}

	void remove(int idx)
	{
		NG_IDX_BOUNDS_CHECK(idx, m_len);
		if (idx == m_len - 1) {
			m_data[--m_len].~T();
			return;
		}

		m_data[idx].~T();
		_move_backward(idx+1, -1);
		m_len--;
	}

	void remove(int begin, int end)
	{
		NG_ASSERT(begin <= end);
		NG_SLICE_BOUNDS_CHECK(begin, m_len);
		NG_SLICE_BOUNDS_CHECK(end, m_len);
		const int len = end - begin;
		if (len == 0)
			return;
		for (int i = begin; i < end; i++)
			m_data[i].~T();
		if (end < m_len)
			_move_backward(begin+len, -len);
		m_len -= len;
	}

	template <typename ...Args>
	void pinsert(int idx, Args &&...args)
	{
		NG_SLICE_BOUNDS_CHECK(idx, m_len);
		_ensure_capacity(1);
		if (idx < m_len) {
			_move_forward(idx, 1);
		}
		new (m_data + idx) T(std::forward<Args>(args)...);
		m_len++;
	}

	void insert(int idx, const T &elem)
	{
		pinsert(idx, elem);
	}

	void insert(int idx, T &&elem)
	{
		pinsert(idx, std::move(elem));
	}

	void insert(int idx, Slice<const T> s)
	{
		NG_SLICE_BOUNDS_CHECK(idx, m_len);
		if (s.length == 0) {
			return;
		}
		if (s.data >= m_data && s.data < m_data + m_len) {
			_self_insert(idx, s);
			return;
		}
		_ensure_capacity(s.length);
		if (idx < m_len)
			_move_forward(idx, s.length);
		for (int i = 0; i < s.length; i++)
			new (m_data + idx + i) T(s.data[i]);
		m_len += s.length;
	}

	template <typename ...Args>
	void pappend(Args &&...args)
	{
		_ensure_capacity(1);
		new (m_data + m_len++) T(std::forward<Args>(args)...);
	}

	T *append()
	{
		_ensure_capacity(1);
		return new (m_data + m_len++) T;
	}

	void append(const T &elem)
	{
		NG_ASSERT(&elem < m_data || &elem >= m_data + m_len);
		pappend(elem);
	}

	void append(T &&elem)
	{
		NG_ASSERT(&elem < m_data || &elem >= m_data + m_len);
		pappend(std::move(elem));
	}

	void append(Slice<const T> s)
	{
		insert(m_len, s);
	}

	T &operator[](int idx)
	{
		NG_IDX_BOUNDS_CHECK(idx, m_len);
		return m_data[idx];
	}

	const T &operator[](int idx) const
	{
		NG_IDX_BOUNDS_CHECK(idx, m_len);
		return m_data[idx];
	}

	T &first() { NG_ASSERT(m_len != 0); return m_data[0]; }
	const T &first() const { NG_ASSERT(m_len != 0); return m_data[0]; }
	T &last() { NG_ASSERT(m_len != 0); return m_data[m_len-1]; }
	const T &last() const { NG_ASSERT(m_len != 0); return m_data[m_len-1]; }

	Slice<T> sub()
	{
		return {m_data, m_len};
	}
	Slice<T> sub(int begin)
	{
		NG_SLICE_BOUNDS_CHECK(begin, m_len);
		return {m_data + begin, m_len - begin};
	}
	Slice<T> sub(int begin, int end)
	{
		NG_ASSERT(begin <= end);
		NG_SLICE_BOUNDS_CHECK(begin, m_len);
		NG_SLICE_BOUNDS_CHECK(end, m_len);
		return {m_data + begin, end - begin};
	}
	Slice<const T> sub() const
	{
		return {m_data, m_len};
	}
	Slice<const T> sub(int begin) const
	{
		NG_SLICE_BOUNDS_CHECK(begin, m_len);
		return {m_data + begin, m_len - begin};
	}
	Slice<const T> sub(int begin, int end) const
	{
		NG_ASSERT(begin <= end);
		NG_SLICE_BOUNDS_CHECK(begin, m_len);
		NG_SLICE_BOUNDS_CHECK(end, m_len);
		return {m_data + begin, end - begin};
	}

	operator Slice<T>() { return {m_data, m_len}; }
	operator Slice<const T>() const { return {m_data, m_len}; }
};

template <typename T>
const T *begin(const Vector<T> &v) { return v.data(); }
template <typename T>
const T *end(const Vector<T> &v) { return v.data()+v.length(); }
template <typename T>
T *begin(Vector<T> &v) { return v.data(); }
template <typename T>
T *end(Vector<T> &v) { return v.data()+v.length(); }
