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
// Behaviour model of vector::_M_insert_overflow for a 60-byte element with a counting copy constructor.
struct E { int v; int pad[14]; static int copies; E(int x) : v(x) { } E(const E& o) : v(o.v) { ++copies; } };
int E::copies = 0;
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
    for (E* p = vec.s; p != vec.f; ++p) p->~E();
    if (vec.s) std::free(vec.s);
    vec.s = ns; vec.f = nf; vec.e = ns + len;
}
int main()
{
    V vec; vec.s = (E*)std::malloc(3 * sizeof(E)); vec.f = vec.s + 3; vec.e = vec.f;
    for (int i = 0; i < 3; ++i) new (vec.s + i) E(i + 1);
    insert_overflow(vec, vec.s + 1, E(9), 2, false);     // [1 9 9 2 3], capacity 6
    if (vec.f - vec.s != 5 || vec.e - vec.s != 6 || vec.s[0].v != 1 || vec.s[1].v != 9 || vec.s[2].v != 9 || vec.s[3].v != 2 || vec.s[4].v != 3)
        { std::printf("00f28400_TEST FAIL\n"); return 1; }
    insert_overflow(vec, vec.f, E(7), 1, true);          // append: [1 9 9 2 3 7], capacity 10
    if (vec.f - vec.s != 6 || vec.e - vec.s != 10 || vec.s[5].v != 7)
        { std::printf("00f28400_TEST FAIL\n"); return 1; }
    std::printf("00f28400_TEST PASS\n");
    return 0;
}