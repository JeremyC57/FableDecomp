#include <cstdio>
#include <cstdlib>
#include <new>
// Behaviour model of STLport vector::reserve: grow only when capacity < n; copy the live elements into the new block,
// destroy and free the old one; finish keeps the old size, end of storage becomes start + n.
struct E { int a, b; static int live; E(int x) : a(x), b(-x) { ++live; } E(const E& o) : a(o.a), b(o.b) { ++live; } ~E() { --live; } };
int E::live = 0;
struct V {
    E* s; E* f; E* e;
    void reserve(unsigned int n)
    {
        if ((unsigned int)(e - s) < n) {
            unsigned int old = (unsigned int)(f - s);
            E* tmp = n ? (E*)std::malloc(n * sizeof(E)) : 0;
            if (s) {
                E* d = tmp; for (E* p = s; p != f; ++p, ++d) new (d) E(*p);
                for (E* p = s; p != f; ++p) p->~E();
                std::free(s);
            }
            s = tmp; f = tmp + old; e = tmp + n;
        }
    }
};
int main()
{
    V v; v.s = v.f = v.e = 0;
    v.reserve(2);
    if (v.s == 0 || v.f != v.s || v.e != v.s + 2) { std::printf("009cb1f0_TEST FAIL\n"); return 1; }
    new (v.f) E(5); ++v.f; new (v.f) E(6); ++v.f;
    E* old = v.s;
    v.reserve(1);                                          // no growth
    if (v.s != old) { std::printf("009cb1f0_TEST FAIL\n"); return 1; }
    v.reserve(8);
    if (v.f - v.s != 2 || v.e - v.s != 8 || v.s[0].a != 5 || v.s[1].b != -6 || E::live != 2) { std::printf("009cb1f0_TEST FAIL\n"); return 1; }
    std::printf("009cb1f0_TEST PASS\n");
    return 0;
}