#include "Core/Utils.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

void assertion_abort(const char *assertion, const char *file,
					int line, const char *func)
{
	fprintf(stderr, "%s:%d: %s: assertion `%s` failed\n",
		file, line, func, assertion);
	abort();
}

void die(const char *msg, ...)
{
	va_list va;
	va_start(va, msg);
	vfprintf(stderr, msg, va);
	va_end(va);
	fprintf(stderr, "\n");
	abort();
}

void warn(const char *msg, ...)
{
	va_list va;
	va_start(va, msg);
	vfprintf(stderr, msg, va);
	va_end(va);
	fprintf(stderr, "\n");
}
