#ifdef _WIN32
#include <Windows.h>
#else
#ifdef __MACH__
#include <time.h>
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#endif // __MACH__
#endif // _WIN32
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
#ifdef __MACH__
	struct timespec t;
	clock_serv_t cclock;
	mach_timespec_t mts;
	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	if (clock_get_time(cclock, &mts) != KERN_SUCCESS) {
		die("clock failure");
	}
	mach_port_deallocate(mach_task_self(), cclock);
	t.tv_sec = mts.tv_sec;
	t.tv_nsec = mts.tv_nsec;
	return (double)t.tv_sec * 1000.0 + (double)t.tv_nsec / 1000000.0;
#else
	struct timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0)
			die("clock failure");
	return (double)t.tv_sec * 1000.0 + (double)t.tv_nsec / 1000000.0;
#endif // __MACH__
#endif // _WIN32
}
