// behaviour test for 0x00A967A0: vector(n) holds n zero elements (none, and no storage, for n == 0)
#include <cstdio>
#include <cstdlib>
#include <new>
struct E { long a; long b; E() : a(0), b(0) { } };
struct V { E* s; E* f; E* e;
    explicit V(unsigned n) : s(0), f(0), e(0) { s = n ? (E*)std::malloc(n * sizeof(E)) : 0; f = s; e = s + n; E z; E* c = s; for (; n > 0; --n, ++c) new (c) E(z); f = c; } };
int main()
{
    V v(3), w(0);
    if (v.f - v.s != 3 || v.e != v.f || v.s[2].a != 0 || v.s[1].b != 0 || w.s != 0 || w.f != 0) { std::printf("00a967a0_TEST FAIL\n"); return 1; }
    std::free(v.s);
    std::printf("00a967a0_TEST PASS\n");
    return 0;
}