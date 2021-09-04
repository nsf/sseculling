# SSE Frustum Culling Demo

A simple demo which runs massive frustum culling (by default 512000 spheres) in various setups.

At the moment it contains two versions of the actual culling code:

1. Naive culling.

   ```c++
   for (int i = 0; i < 6; i++) {
       const Plane &p = planes[i];
       if (dot(p.n, s.center) + p.d < -s.radius)
           return true;
   }
   return false;
   ```

2. SSE culling.

   This is a simple SSE version of the algorithm inspired by [Culling the Battlefield](http://www.frostbite.com/2011/04/culling-the-battlefield-data-oriented-design-in-practice/). The algorithm itself with minor modifications is taken from [here](http://www.hmrengine.com/blog/?p=490). I hope the author doesn't mind, after all it's just a simple parallel dot product calculation on SSE.

   With SSE using SoA data structures you can test a sphere against 4 planes at a time. Frustum has 6, and the algorithm wastes 2 planes.

The demo should work on both linux (gcc 5.2/clang 3.6) and windows (msvc++ 2015). But you need to install cmake on windows to generate visual studio files.

There is a command line options to explore:

- `-v` Enables verbose output. Also prints ASCII slice of the sphere field, for verification purposes.
- `-s <N>` Overrides the size of the sphere field. That's just one dimensions, the results size of the field is N x N x N.

## Results

Example output on my machine (`i5-3470`, `linux`, `x86_64`, `gcc 5.2`):

```
[nsf @ build]$ ./sseculling
Data size: 80x80x80 (512000 objects, 8192000 bytes)
'Naive culling / structured data' done in 10 runs, average: 3.394078ms
'Naive culling / random data' done in 10 runs, average: 8.155710ms
'SSE culling / structured data' done in 10 runs, average: 2.353994ms
'SSE culling / random data' done in 10 runs, average: 2.345242ms
----------------------------------------
'SSE culling / chunks / structured data / 512 per chunk (w/o  prefetch)' done in 10 runs, average: 2.525645ms
'SSE culling / chunks / random data     / 512 per chunk (w/o  prefetch)' done in 10 runs, average: 2.531772ms
'SSE culling / chunks / random data     / 512 per chunk (with prefetch)' done in 10 runs, average: 2.502262ms
----------------------------------------
'SSE culling / chunks / structured data / 256 per chunk (w/o  prefetch)' done in 10 runs, average: 2.645812ms
'SSE culling / chunks / random data     / 256 per chunk (w/o  prefetch)' done in 10 runs, average: 2.656828ms
'SSE culling / chunks / random data     / 256 per chunk (with prefetch)' done in 10 runs, average: 2.605833ms
----------------------------------------
'SSE culling / chunks / structured data / 128 per chunk (w/o  prefetch)' done in 10 runs, average: 2.743400ms
'SSE culling / chunks / random data     / 128 per chunk (w/o  prefetch)' done in 10 runs, average: 2.889656ms
'SSE culling / chunks / random data     / 128 per chunk (with prefetch)' done in 10 runs, average: 2.806787ms
----------------------------------------
'SSE culling / chunks / structured data /  64 per chunk (w/o  prefetch)' done in 10 runs, average: 2.819107ms
'SSE culling / chunks / random data     /  64 per chunk (w/o  prefetch)' done in 10 runs, average: 3.470009ms
'SSE culling / chunks / random data     /  64 per chunk (with prefetch)' done in 10 runs, average: 3.370178ms
----------------------------------------
'SSE culling / chunks / structured data /  32 per chunk (w/o  prefetch)' done in 10 runs, average: 2.648684ms
'SSE culling / chunks / random data     /  32 per chunk (w/o  prefetch)' done in 10 runs, average: 4.479559ms
'SSE culling / chunks / random data     /  32 per chunk (with prefetch)' done in 10 runs, average: 4.120239ms
----------------------------------------
'SSE culling / chunks / structured data /   8 per chunk (w/o  prefetch)' done in 10 runs, average: 3.001685ms
'SSE culling / chunks / random data     /   8 per chunk (w/o  prefetch)' done in 10 runs, average: 9.516912ms
'SSE culling / chunks / random data     /   8 per chunk (with prefetch)' done in 10 runs, average: 5.933696ms

```

Few comments on results:

1. Naive algorithm can fool you into thinking that it's almost as fast as SSE one, that's due to branch prediction friendly data. If you shuffle the data however, the truth comes out.

2. SSE version is roughly 4 times faster than naive version.

3. If we break data into chunks and randomize them, you can clearly see cache miss effects. The effects disappear starting from 128 elements per chunk.

4. On very fragmented data using `_mm_prefetch` instruction helps quite a bit.

## Results on a different machine

This time it's AMD CPU, note how prefetching makes way more difference here than on intel. Very interesting.

Example output on my latest machine (`amd threadripper 1950x`, `linux`, `x86_64`, `clang 8.0.1`):

```
nsf@crey ~/p/s/build> ./sseculling
Data size: 80x80x80 (512000 objects, 8192000 bytes)
'Naive culling / structured data' done in 10 runs, average: 2.186710ms
'Naive culling / random data' done in 10 runs, average: 8.848435ms
'SSE culling / structured data' done in 10 runs, average: 1.445463ms
'SSE culling / random data' done in 10 runs, average: 1.467520ms
----------------------------------------
'SSE culling / chunks / structured data / 512 per chunk (w/o  prefetch)' done in 10 runs, average: 1.854928ms
'SSE culling / chunks / random data     / 512 per chunk (w/o  prefetch)' done in 10 runs, average: 1.798940ms
'SSE culling / chunks / random data     / 512 per chunk (with prefetch)' done in 10 runs, average: 1.522028ms
----------------------------------------
'SSE culling / chunks / structured data / 256 per chunk (w/o  prefetch)' done in 10 runs, average: 2.149564ms
'SSE culling / chunks / random data     / 256 per chunk (w/o  prefetch)' done in 10 runs, average: 2.022723ms
'SSE culling / chunks / random data     / 256 per chunk (with prefetch)' done in 10 runs, average: 1.733204ms
----------------------------------------
'SSE culling / chunks / structured data / 128 per chunk (w/o  prefetch)' done in 10 runs, average: 2.108009ms
'SSE culling / chunks / random data     / 128 per chunk (w/o  prefetch)' done in 10 runs, average: 2.297155ms
'SSE culling / chunks / random data     / 128 per chunk (with prefetch)' done in 10 runs, average: 2.026920ms
----------------------------------------
'SSE culling / chunks / structured data /  64 per chunk (w/o  prefetch)' done in 10 runs, average: 2.027192ms
'SSE culling / chunks / random data     /  64 per chunk (w/o  prefetch)' done in 10 runs, average: 3.048898ms
'SSE culling / chunks / random data     /  64 per chunk (with prefetch)' done in 10 runs, average: 2.619212ms
----------------------------------------
'SSE culling / chunks / structured data /  32 per chunk (w/o  prefetch)' done in 10 runs, average: 2.183860ms
'SSE culling / chunks / random data     /  32 per chunk (w/o  prefetch)' done in 10 runs, average: 4.267031ms
'SSE culling / chunks / random data     /  32 per chunk (with prefetch)' done in 10 runs, average: 3.265937ms
----------------------------------------
'SSE culling / chunks / structured data /   8 per chunk (w/o  prefetch)' done in 10 runs, average: 1.990538ms
'SSE culling / chunks / random data     /   8 per chunk (w/o  prefetch)' done in 10 runs, average: 10.015637ms
'SSE culling / chunks / random data     /   8 per chunk (with prefetch)' done in 10 runs, average: 8.100875ms
```

## Results on a yet another machine

AMD CPU again. Best of the best as of time of the commit.

Example output on my latest machine (`amd ryzen 9 5950x`, `linux`, `x86_64`, `clang 12.0.1`):

```
~/projects/sseculling/build> ./sseculling
Data size: 80x80x80 (512000 objects, 8192000 bytes)
'Naive culling / structured data' done in 10 runs, average: 1.625211ms
'Naive culling / random data' done in 10 runs, average: 6.584432ms
'SSE culling / structured data' done in 10 runs, average: 1.214213ms
'SSE culling / random data' done in 10 runs, average: 1.212837ms
----------------------------------------
'SSE culling / chunks / structured data / 512 per chunk (w/o  prefetch)' done in 10 runs, average: 1.216788ms
'SSE culling / chunks / random data     / 512 per chunk (w/o  prefetch)' done in 10 runs, average: 1.229120ms
'SSE culling / chunks / random data     / 512 per chunk (with prefetch)' done in 10 runs, average: 1.228033ms
----------------------------------------
'SSE culling / chunks / structured data / 256 per chunk (w/o  prefetch)' done in 10 runs, average: 1.213698ms
'SSE culling / chunks / random data     / 256 per chunk (w/o  prefetch)' done in 10 runs, average: 1.260839ms
'SSE culling / chunks / random data     / 256 per chunk (with prefetch)' done in 10 runs, average: 1.241638ms
----------------------------------------
'SSE culling / chunks / structured data / 128 per chunk (w/o  prefetch)' done in 10 runs, average: 1.200609ms
'SSE culling / chunks / random data     / 128 per chunk (w/o  prefetch)' done in 10 runs, average: 1.264215ms
'SSE culling / chunks / random data     / 128 per chunk (with prefetch)' done in 10 runs, average: 1.258731ms
----------------------------------------
'SSE culling / chunks / structured data /  64 per chunk (w/o  prefetch)' done in 10 runs, average: 1.262710ms
'SSE culling / chunks / random data     /  64 per chunk (w/o  prefetch)' done in 10 runs, average: 1.405424ms
'SSE culling / chunks / random data     /  64 per chunk (with prefetch)' done in 10 runs, average: 1.350807ms
----------------------------------------
'SSE culling / chunks / structured data /  32 per chunk (w/o  prefetch)' done in 10 runs, average: 1.112343ms
'SSE culling / chunks / random data     /  32 per chunk (w/o  prefetch)' done in 10 runs, average: 1.405099ms
'SSE culling / chunks / random data     /  32 per chunk (with prefetch)' done in 10 runs, average: 1.490859ms
----------------------------------------
'SSE culling / chunks / structured data /   8 per chunk (w/o  prefetch)' done in 10 runs, average: 1.246730ms
'SSE culling / chunks / random data     /   8 per chunk (w/o  prefetch)' done in 10 runs, average: 2.170331ms
'SSE culling / chunks / random data     /   8 per chunk (with prefetch)' done in 10 runs, average: 1.999654ms
```