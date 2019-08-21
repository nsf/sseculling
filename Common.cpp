#include "Common.h"
#include "Timer.h"
#include "Core/Vector.h"
#include <stdio.h>

void parse_args(Config *config, int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (strcmp(arg, "-v") == 0) {
            config->verbose = true;
        } else if (strcmp(arg, "-s") == 0) {
            config->data_size = atoi(argv[++i]);
        }
    }
}

void print_results(Slice<const uint32_t> bits, const Config &config)
{
    if (!config.verbose)
        return;

    const int size = config.data_size;
    const int mid = size / 2;
    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            const int i = offset_3d(Vec3i(x, mid, z), Vec3i(size));
            const int ri = i / 32;
            const int shift = i % 32;
            if (bits[ri] & (1U << shift)) {
                printf(". ");
            } else {
                printf("# ");
            }
        }
        printf("\n");
    }
}

void measure(Func<void()> f, int warmup, int runs, const char *name, const Config &config)
{
    Vector<double> results(runs);
    for (int i = 0; i < warmup; i++) {
        f();
    }
    double average = get_time_milliseconds();
    for (int i = 0; i < runs; i++) {
        const double begin = get_time_milliseconds();
        f();
        results[i] = get_time_milliseconds() - begin;
    }
    average = (get_time_milliseconds() - average) / runs;
    printf("'%s' done in %d runs, average: %fms\n", name, runs, average);
    if (config.verbose) {
        printf("per-run info:\n");
        for (int i = 0; i < runs; i++)
            printf(" [%d] %fms\n", i, results[i]);
    }
}

void naive_cull(Slice<uint32_t> results, Slice<const Sphere> spheres, const Frustum &f)
{
    for (int i = 0, n = spheres.length; i < n; i++) {
        const Sphere &s = spheres.data[i];
        const uint32_t result = f.cull(s) & 1;
        const int ri = i / 32;
        const int shift = i % 32;
        results.data[ri] |= result << shift;
    }
}

void sse_cull(Slice<uint32_t> results, Slice<const Sphere> spheres, const Frustum &f)
{
    // we negate everything because we use this formula to cull:
    //   dot(-p.n, s.center) - p.d > s.radius
    // it's equivalent to:
    //   dot(p.n, s.center) + p.d < -s.radius
    // but no need to negate sphere radius
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

    for (int i = 0, n = spheres.length; i < n; i++) {
        // Load sphere into SSE register.
        const __m128 s = _mm_load_ps(reinterpret_cast<const float*>(spheres.data+i));
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
        //    a b c d -> c d c d
        // 2. then we OR it with the existing value (ignoring 2 upper floats)
        //    a b | c d = A B
        // 3. and then we OR it again ignoring all but 1 lowest float:
        //    A | B = R
        // Result is written in the lowest float.
        r = _mm_or_ps(r, _mm_movehl_ps(r, r));
        r = _mm_or_ps(r, simd_splat_y(r));

        uint32_t result;
        _mm_store_ss((float*)&result, r);

        // And write the result back to bit buffer.
        const int ri = i / 32;
        const int shift = i % 32;
        results.data[ri] |= (result & 1) << shift;
    }
}
