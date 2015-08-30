#include "Common.h"
#include "Core/Vector.h"
#include "Math/Sphere.h"
#include "Math/Frustum.h"
#include <chrono>
#include <random>
#include <algorithm>

struct Data {
    Vector<Sphere> spheres = Vector<Sphere>(&sse_allocator);
    Vector<uint32_t> results;

    // Maps 3d position (offset_3d(Vec3i(x, y, z), Vec3i(data_size)) to actual
    // sphere position.
    Vector<int> mapping;
};

static Data generate_data(DataType data_type, const Config &config)
{
    Data data;
    const int half_size = config.data_size/2;
    for (int z = 0; z < config.data_size; z++) {
    for (int y = 0; y < config.data_size; y++) {
    for (int x = 0; x < config.data_size; x++) {
        const Vec3i p = (Vec3i(x, y, z) - Vec3i(half_size)) * Vec3i(2);
        data.spheres.pappend(ToVec3f(p), 1.0f);
    }}}

    for (int i = 0; i < data.spheres.length(); i++)
        data.mapping.append(i);

    data.results.resize((data.spheres.length() + 31) / 32);
    fill<uint32_t>(data.results, 0);

    // If random data is requested, shuffle the mapping and move the spheres.
    if (data_type == Random) {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(data.mapping.data(), data.mapping.data() + data.mapping.length(),
            std::default_random_engine(seed));
        Vector<Sphere> spheres_tmp(&sse_allocator);
        spheres_tmp.resize(data.spheres.length());
        for (int i = 0; i < data.spheres.length(); i++) {
            spheres_tmp[data.mapping[i]] = data.spheres[i];
        }
        data.spheres = std::move(spheres_tmp);
    }
    return data;
}

static Vector<uint32_t> get_results(const Data &data)
{
    Vector<uint32_t> out(data.results.length());
    fill<uint32_t>(out, 0);
    for (int i = 0, n = data.spheres.length(); i < n; i++) {
        const int i2 = data.mapping[i];
        const int ri1 = i / 32;
        const int shift1 = i % 32;
        const int ri2 = i2 / 32;
        const int shift2 = i2 % 32;
        const uint32_t result = (data.results[ri2] & (1U << shift2)) != 0;
        out[ri1] |= (result & 1) << shift1;
    }
    return out;
}

void do_arrays(const Config &config)
{
    const Frustum f = Frustum_Perspective(75.0f, 1.333f, 0.5f, 100.0f);
    Data data;

    data = generate_data(Structured, config);
    measure([&]{ naive_cull(data.results, data.spheres, f); }, 50, 10, "Naive culling / structured data", config);
    print_results(get_results(data), config);

    data = generate_data(Random, config);
    measure([&]{ naive_cull(data.results, data.spheres, f); }, 50, 10, "Naive culling / random data", config);
    print_results(get_results(data), config);

    data = generate_data(Structured, config);
    measure([&]{ sse_cull(data.results, data.spheres, f); }, 50, 10, "SSE culling / structured data", config);
    print_results(get_results(data), config);

    data = generate_data(Random, config);
    measure([&]{ sse_cull(data.results, data.spheres, f); }, 50, 10, "SSE culling / random data", config);
    print_results(get_results(data), config);
}
