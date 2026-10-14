#include <cstdio>

struct Pool { long n; long h[4]; Pool() : n(0) {} void Lock(long x) { h[n++] = x; } };
struct Prim { unsigned char body[0xa0]; };
struct Mesh { Pool* pool; long a, b, c; Prim* prims;
    Prim* LockPrimitive(long i) { pool->Lock(a); pool->Lock(b); if (c != -1) pool->Lock(c); return &prims[i]; } };
int main()
{
    Pool pool; Prim prims[3]; Mesh m = { &pool, 10, 11, -1, prims };
    if (m.LockPrimitive(2) != &prims[2] || pool.n != 2 || pool.h[1] != 11) { std::printf("00a89250_TEST FAIL\n"); return 1; }
    m.c = 12; m.LockPrimitive(0); if (pool.n != 5 || pool.h[4] != 12) { std::printf("00a89250_TEST FAIL\n"); return 1; }
    std::printf("00a89250_TEST PASS\n");
    return 0;
}