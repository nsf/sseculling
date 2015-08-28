#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif
#include "Core/Utils.h"

double get_time_milliseconds()
{
#ifdef _WIN32
	static double freq = 0.0;
	if (freq == 0.0) {
		LARGE_INTEGER li;
		if (!QueryPerformanceFrequency(&li))
			die("clock failure");
		freq = (double)li.QuadPart / 1000.0;
	}
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (double)li.QuadPart / freq;
#else
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
        die("clock failure");
    return (double)t.tv_sec * 1000.0 + (double)t.tv_nsec / 1000000.0;
#endif
}
