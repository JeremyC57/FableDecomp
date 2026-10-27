#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour model of vector::_M_insert_overflow for a 12-byte pair.
struct E { unsigned long k; long a, b; E(unsigned long x) : k(x), a(0), b(0) { } E(const E& o) : k(o.k), a(o.a), b(o.b) { } };
struct V { E* s; E* f; E* e; };
static void insert_overflow(V& vec, E* pos, const E& x, unsigned int fill_len, bool atend)
{
    unsigned int old = (unsigned int)(vec.f - vec.s);
    unsigned int len = old + (old < fill_len ? fill_len : old);
    E* ns = len ? (E*)std::malloc(len * sizeof(E)) : 0;
    E* nf = ns;
    for (E* p = vec.s; p != pos; ++p, ++nf) new (nf) E(*p);
    if (fill_len == 1) { new (nf) E(x); ++nf; }
    else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) E(x);
    if (!atend) for (E* p = pos; p != vec.f; ++p, ++nf) new (nf) E(*p);
    if (vec.s) std::free(vec.s);
    vec.s = ns; vec.f = nf; vec.e = ns + len;
}
int main()
{
    V v; v.s = (E*)std::malloc(2 * sizeof(E)); new (v.s) E(1); new (v.s + 1) E(3); v.f = v.s + 2; v.e = v.f;
    insert_overflow(v, v.s + 1, E(2), 1, false);
    if (v.f - v.s != 3 || v.e - v.s != 4 || v.s[1].k != 2 || v.s[2].k != 3) { std::printf("009b60d0_TEST FAIL\n"); return 1; }
    insert_overflow(v, v.f, E(9), 3, true);
    if (v.f - v.s != 6 || v.s[5].k != 9) { std::printf("009b60d0_TEST FAIL\n"); return 1; }
    std::printf("009b60d0_TEST PASS\n");
    return 0;
}