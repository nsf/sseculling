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
'Naive culling / structured data' done in 10 runs, average: 3.404757ms
'Naive culling / random data' done in 10 runs, average: 8.190406ms
'SSE culling / structured data' done in 10 runs, average: 2.191884ms
'SSE culling / random data' done in 10 runs, average: 2.203724ms
```

Few comments on results:

1. Naive algorithm can fool you into thinking that it's almost as fast as SSE one, that's due to branch prediction friendly data. If you shuffle the data however, the truth comes out.

2. SSE version is roughly 4 times faster than naive version.
