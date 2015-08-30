#pragma once

#include <emmintrin.h>
#include "Math/Vec.h"
#include "Core/Slice.h"
#include "Core/Func.h"
#include "Math/Sphere.h"
#include "Math/Frustum.h"

enum DataType {
    Structured,
    Random,
};

struct Config {
    int data_size = 80;
    bool verbose = false;
};

static inline int offset_3d(const Vec3i &p, const Vec3i &size)
{
    return (p.z * size.y + p.y) * size.x + p.x;
}

static inline __m128 simd_set(float x, float y, float z, float w)
{
    return _mm_set_ps(w, z, y, x);
}

static inline __m128 simd_splat_x(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(0, 0, 0, 0)); }
static inline __m128 simd_splat_y(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(1, 1, 1, 1)); }
static inline __m128 simd_splat_z(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(2, 2, 2, 2)); }
static inline __m128 simd_splat_w(__m128 v) { return _mm_shuffle_ps(v, v, _MM_SHUFFLE(3, 3, 3, 3)); }

static inline __m128 simd_madd(__m128 a, __m128 b, __m128 c)
{
    return _mm_add_ps(_mm_mul_ps(a, b), c);
}

void parse_args(Config *config, int argc, char **argv);
void print_results(Slice<const uint32_t> bits, const Config &config);
void measure(Func<void()> f, int warmup, int runs, const char *name, const Config &config);

void naive_cull(Slice<uint32_t> results, Slice<const Sphere> spheres, const Frustum &f);
void sse_cull(Slice<uint32_t> results, Slice<const Sphere> spheres, const Frustum &f);

void do_arrays(const Config &config);
void do_chunks(const Config &config);
