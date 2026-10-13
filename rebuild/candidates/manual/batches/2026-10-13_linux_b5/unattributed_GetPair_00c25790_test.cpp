#include <cstdio>
struct P { long i; float v; P(long a, float b) : i(a), v(b) {} }; struct O { long i; float v; P Get() { return P(i, v); } };
int main()
{
    O o; o.i = 4; o.v = 0.5f; P r = o.Get(); if (r.i != 4 || r.v != 0.5f) { std::printf("00c25790_TEST FAIL\n"); return 1; }

    std::printf("00c25790_TEST PASS\n");
    return 0;
}
