#include "Core/Slice.h"

#define _SLICE_CONST_CHAR_OPERATOR(op)                           \
bool operator op(Slice<const char> lhs, Slice<const char> rhs) { \
	return operator op <const char, const char>(lhs, rhs);        \
}

_SLICE_CONST_CHAR_OPERATOR(==)
_SLICE_CONST_CHAR_OPERATOR(<)
_SLICE_CONST_CHAR_OPERATOR(!=)
_SLICE_CONST_CHAR_OPERATOR(>=)
_SLICE_CONST_CHAR_OPERATOR(<=)
_SLICE_CONST_CHAR_OPERATOR(>)

#undef _SLICE_CONST_CHAR_OPERATOR

int compute_hash(Slice<const char> s)
{
	constexpr unsigned M0 = 2860486313U;
	constexpr unsigned M1 = 3267000013U;
	unsigned hash = M0;
	for (const auto &b : s)
		hash = (hash ^ b) * M1;
	return hash;
}

int compute_hash(const char *s)
{
	return compute_hash(Slice<const char>(s));
}

int compute_hash(int i)
{
	return compute_hash(slice_cast<const char>(Slice<int>(&i, 1)));
}
