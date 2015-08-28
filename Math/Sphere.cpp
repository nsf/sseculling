#include "Math/Sphere.h"

Sphere transform(const Sphere &in, const Transform &tr)
{
	Sphere out = in;
	out.center = transform(in.center, tr);
	return out;
}
