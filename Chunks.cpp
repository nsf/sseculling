#include "Common.h"
#include "Core/Vector.h"
#include "Core/UniquePtr.h"
#include "Math/Sphere.h"
#include "Math/Frustum.h"
#include <chrono>
#include <random>
#include <algorithm>

struct Chunk {
    Vector<Sphere> spheres = Vector<Sphere>(&sse_allocator);
    Vector<uint32_t> results;

    Chunk(int max)
    {
            spheres.reserve(max);
            results.resize(max + 31 / 32);
            fill<uint32_t>(results, 0);
    }
};

struct Data {
    Vector<UniquePtr<Chunk>> chunks_ordered;
    Vector<Chunk*> chunks;
};

static Data generate_data(DataType data_type, const Config &config, int max)
{
    Data data;
    data.chunks_ordered.pappend(new (OrDie) Chunk(max));
    Chunk *c = data.chunks_ordered.last().get();
    const int half_size = config.data_size/2;
    for (int z = 0; z < config.data_size; z++) {
    for (int y = 0; y < config.data_size; y++) {
    for (int x = 0; x < config.data_size; x++) {
        const Vec3i p = (Vec3i(x, y, z) - Vec3i(half_size)) * Vec3i(2);
        c->spheres.pappend(ToVec3f(p), 1.0f);
        if (c->spheres.length() == max) {
            data.chunks_ordered.pappend(new (OrDie) Chunk(max));
            c = data.chunks_ordered.last().get();
        }
    }}}

    if (data.chunks_ordered.last()->spheres.length() == 0)
        data.chunks_ordered.resize(data.chunks_ordered.length() - 1);

    data.chunks.resize(data.chunks_ordered.length());
    for (int i = 0; i < data.chunks.length(); i++) {
        data.chunks[i] = data.chunks_ordered[i].get();
    }

    if (data_type == Random) {
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(data.chunks.data(), data.chunks.data() + data.chunks.length(),
            std::default_random_engine(seed));
    }
    return data;
}

static Vector<uint32_t> get_results(const Data &data)
{
    int count = 0;
    for (const auto &c : data.chunks_ordered)
        count += c->spheres.length();

    Vector<uint32_t> out(count + 31 / 32);
    fill<uint32_t>(out, 0);
    int out_i = 0;
    for (const auto &c : data.chunks_ordered) {
        for (int i = 0, n = c->spheres.length(); i < n; i++) {
            const int ri = i / 32;
            const int shift = i % 32;
            const int out_ri = out_i / 32;
            const int out_shift = out_i % 32;
            const uint32_t result = (c->results[ri] & (1U << shift)) != 0;
            out[out_ri] |= (result & 1) << out_shift;
            out_i++;
        }
    }
    return out;
}

static void sse_cull_data(Data *data, const Frustum &f)
{
    for (const auto &c : data->chunks)
        sse_cull(c->results, c->spheres, f);
}

static void sse_cull_data_prefetch(Data *data, const Frustum &f)
{
    for (int i = 0, n = data->chunks.length(); i < n; i++) {
        if (i != n-1) {
            // Tried all hints there, NTA works best for very fragmented data.
            _mm_prefetch(data->chunks.data()[i+1]->spheres.data(), _MM_HINT_NTA);
            _mm_prefetch(data->chunks.data()[i+1]->results.data(), _MM_HINT_NTA);
        }
        const auto &c = data->chunks.data()[i];
        sse_cull(c->results, c->spheres, f);
    }
}

void do_chunks(const Config &config)
{
    const Frustum f = Frustum_Perspective(75.0f, 1.333f, 0.5f, 100.0f);
    Data data;

    auto just_do_it = [&](int N) {
        char buf[4096];
        snprintf(buf, sizeof(buf), "%03d", N);

        snprintf(buf, sizeof(buf), "SSE culling / chunks / structured data / %3d per chunk (w/o  prefetch)", N);
        data = generate_data(Structured, config, N);
        measure([&]{ sse_cull_data(&data, f); }, 50, 10, buf, config);
        print_results(get_results(data), config);

        snprintf(buf, sizeof(buf), "SSE culling / chunks / random data     / %3d per chunk (w/o  prefetch)", N);
        data = generate_data(Random, config, N);
        measure([&]{ sse_cull_data(&data, f); }, 50, 10, buf, config);
        print_results(get_results(data), config);

        snprintf(buf, sizeof(buf), "SSE culling / chunks / random data     / %3d per chunk (with prefetch)", N);
        data = generate_data(Random, config, N);
        measure([&]{ sse_cull_data_prefetch(&data, f); }, 50, 10, buf, config);
        print_results(get_results(data), config);
    };

    const int tries[] = {512, 256, 128, 64, 32, 8};
    for (int t : tries) {
        printf("----------------------------------------\n");
        just_do_it(t);
    }
}
