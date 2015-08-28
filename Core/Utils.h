#pragma once

//----------------------------------------------------------------------
// Configuration
//----------------------------------------------------------------------

#define NG_ASSERTION_ENABLED

//----------------------------------------------------------------------
//----------------------------------------------------------------------

void assertion_abort(const char *assertion, const char *file,
	int line, const char *func);

#ifdef _WIN32
 #define PRETTY_FUNCTION __FUNCTION__
#else
 #define PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

#ifdef NG_ASSERTION_ENABLED
	#define NG_ASSERT(expr)                  \
	do {                                     \
		if (!(expr)) {                       \
			assertion_abort(#expr,           \
				__FILE__, __LINE__,          \
				PRETTY_FUNCTION);            \
		}                                    \
	} while (0)
#else
	#define NG_ASSERT(expr) ((void)0)
#endif

#define NG_SLICE_BOUNDS_CHECK(index, length) \
	NG_ASSERT((unsigned)(index) <= (unsigned)(length))

#define NG_IDX_BOUNDS_CHECK(index, length) \
	NG_ASSERT((unsigned)(index) < (unsigned)(length))

void die(const char *msg, ...);
void warn(const char *msg, ...);

#define NG_DELETE_COPY(Type) \
	Type(const Type&) = delete; \
	Type &operator=(const Type&) = delete

#define NG_DELETE_MOVE(Type) \
	Type(Type&&) = delete; \
	Type &operator=(Type&&) = delete

#define NG_DELETE_COPY_AND_MOVE(Type) \
	NG_DELETE_COPY(Type); \
	NG_DELETE_MOVE(Type)

#define AS_SLAVE_PTR(T, ptr) (*reinterpret_cast<SlavePtr<T>*>(&(ptr)))
