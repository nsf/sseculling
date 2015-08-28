#include <stdio.h>
#include <algorithm>
#include <random>
#include <chrono>
#include <emmintrin.h>
#include "Core/Utils.h"
#include "Core/Vector.h"
#include "Core/Func.h"
#include "Math/Sphere.h"
#include "Math/Frustum.h"
#include "timer.h"

static bool verbose = false;
static int data_size = 80;
static Vector<Sphere> spheres(&sse_allocator);
static Vector<uint32_t> results;

// Maps 3d position (offset_3d(Vec3i(x, y, z), Vec3i(data_size)) to actual
// sphere position.
static Vector<int> mapping;

static inline int offset_3d(const Vec3i &p, const Vec3i &size)
{
    return (p.z * size.y + p.y) * size.x + p.x;
}

static void generate_data(bool shuffle = false)
{
    spheres.clear();
    const int half_size = data_size/2;
    for (int z = 0; z < data_size; z++) {
    for (int y = 0; y < data_size; y++) {
    for (int x = 0; x < data_size; x++) {
        const Vec3i p = (Vec3i(x, y, z) - Vec3i(half_size)) * Vec3i(2);
        spheres.pappend(ToVec3f(p), 1.0f);
    }}}

    mapping.clear();
    for (int i = 0; i < spheres.length(); i++)
        mapping.append(i);

    results.resize((spheres.length() + 31) / 32);
    fill<uint32_t>(results, 0);

    if (shuffle) {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(mapping.data(), mapping.data() + mapping.length(),
            std::default_random_engine(seed));
        Vector<Sphere> spheres_tmp(&sse_allocator);
        spheres_tmp.resize(spheres.length());
        for (int i = 0; i < spheres.length(); i++) {
            spheres_tmp[mapping[i]] = spheres[i];
        }
        spheres = std::move(spheres_tmp);
    }
}

static void naive_cull(const Frustum &f)
{
    for (int i = 0, n = spheres.length(); i < n; i++) {
        const Sphere &s = spheres.data()[i];
        const uint32_t result = f.cull(s) & 1;
        const int ri = i / 32;
        const int shift = i % 32;
        results.data()[ri] |= result << shift;
    }
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

static void sse_cull(const Frustum &f)
{
    // we negate everything because we use this formula to cull:
    //   dot(-p.n, s.center) - p.d > s.radius
    // it's equivalent to:
    //   dot(p.n, s.center) + p.d < -s.radius
    // but no need to negate radius

    const __m128 plane_components[8] = {
        simd_set(-f.planes[0].n.x, -f.planes[1].n.x, -f.planes[2].n.x, -f.planes[3].n.x),
        simd_set(-f.planes[0].n.y, -f.planes[1].n.y, -f.planes[2].n.y, -f.planes[3].n.y),
        simd_set(-f.planes[0].n.z, -f.planes[1].n.z, -f.planes[2].n.z, -f.planes[3].n.z),
        simd_set(-f.planes[0].d,   -f.planes[1].d,   -f.planes[2].d,   -f.planes[3].d),
        simd_set(-f.planes[4].n.x, -f.planes[5].n.x, -f.planes[4].n.x, -f.planes[5].n.x),
        simd_set(-f.planes[4].n.y, -f.planes[5].n.y, -f.planes[4].n.y, -f.planes[5].n.y),
        simd_set(-f.planes[4].n.z, -f.planes[5].n.z, -f.planes[4].n.z, -f.planes[5].n.z),
        simd_set(-f.planes[4].d,   -f.planes[5].d,   -f.planes[4].d,   -f.planes[5].d),
    };
    for (int i = 0, n = spheres.length(); i < n; i++) {
        // Load sphere into SSE register.
        const __m128 s = _mm_load_ps(reinterpret_cast<float*>(spheres.data()+i));
        const __m128 xxxx = simd_splat_x(s);
        const __m128 yyyy = simd_splat_y(s);
        const __m128 zzzz = simd_splat_z(s);
        const __m128 rrrr = simd_splat_w(s);

        __m128 v, r;
        // Move sphere center to plane normal space and make it relative to plane.
        // dot(p.n, s) + p.d
        v = simd_madd(xxxx, plane_components[0], plane_components[3]);
        v = simd_madd(yyyy, plane_components[1], v);
        v = simd_madd(zzzz, plane_components[2], v);

        // One of r floats will be set to 0xFFFFFFFF if sphere is outside of the frustum.
        r = _mm_cmpgt_ps(v, rrrr);

        // Same for second set of planes.
        v = simd_madd(xxxx, plane_components[4], plane_components[7]);
        v = simd_madd(yyyy, plane_components[5], v);
        v = simd_madd(zzzz, plane_components[6], v);

        r = _mm_or_ps(r, _mm_cmpgt_ps(v, rrrr));

        // Shuffle and extract the result:
        // 1. movehl(r, r) does this (we're interested in 2 lower floats):
        //    aaaa bbbb cccc dddd -> cccc dddd cccc dddd
        // 2. then we OR it with the existing value (ignoring 2 upper floats)
        //    aaaa bbbb | cccc dddd = AAAA BBBB
        // 3. and then we OR it again ignoring all but 1 lowest float:
        //    AAAA | BBBB = RESULT
        // Result is written in the lowest float.
        r = _mm_or_ps(r, _mm_movehl_ps(r, r));
        r = _mm_or_ps(r, simd_splat_y(r));

        uint32_t result;
        _mm_store_ss((float*)&result, r);

        // And write the result back to bit buffer.
        const int ri = i / 32;
        const int shift = i % 32;
        results.data()[ri] |= (result & 1) << shift;
    }
}

static void print_result_slice()
{
    if (!verbose)
        return;

    const int mid = data_size / 2;
    for (int y = 0; y < data_size; y++) {
        for (int x = 0; x < data_size; x++) {
            const int i = mapping[offset_3d(Vec3i(x, mid, y), Vec3i(data_size))];
            const int ri = i / 32;
            const int shift = i % 32;
            if (results[ri] & (1U << shift)) {
                printf(". ");
            } else {
                printf("# ");
            }
        }
        printf("\n");
    }
}

static void measure(Func<void()> f, int warmup, int runs, const char *name)
{
    Vector<double> results(runs);
    for (int i = 0; i < warmup; i++) {
        f();
    }
    for (int i = 0; i < runs; i++) {
        const double begin = get_time_milliseconds();
        f();
        results[i] = get_time_milliseconds() - begin;
    }
    double average = 0.0;
    for (double result : results)
        average += result;
    average /= runs;
    printf("'%s' done in %d runs, average: %fms\n", name, runs, average);
    if (verbose) {
        printf("per-run info:\n");
        for (int i = 0; i < runs; i++)
            printf(" [%d] %fms\n", i, results[i]);
    }
}

static void parse_args(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (strcmp(arg, "-v") == 0) {
            verbose = true;
        } else if (strcmp(arg, "-s") == 0) {
            data_size = atoi(argv[++i]);
        }
    }
}

int main(int argc, char **argv)
{
    parse_args(argc, argv);

    printf("Data size: %dx%dx%d (%d objects, %zu bytes)\n",
        data_size, data_size, data_size,
        volume(Vec3i(data_size)),
        volume(Vec3i(data_size)) * sizeof(Sphere));

    const Frustum f = Frustum_Perspective(75.0f, 1.333f, 0.5f, 100.0f);

    generate_data(/* shuffle */ false);
    measure([&]{ naive_cull(f); }, 50, 10, "Naive culling / structured data");
    print_result_slice();

    generate_data(/* shuffle */ true);
    measure([&]{ naive_cull(f); }, 50, 10, "Naive culling / random data");
    print_result_slice();

    generate_data(/* shuffle */ false);
    measure([&]{ sse_cull(f); }, 50, 10, "SSE culling / structured data");
    print_result_slice();

    generate_data(/* shuffle */ true);
    measure([&]{ sse_cull(f); }, 50, 10, "SSE culling / random data");
    print_result_slice();
    return 0;
}
