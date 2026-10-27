#include <cstdio>
#include <cstdlib>
#include <cstring>
// Behaviour model of STLport vector::operator= over 16-byte elements: grow (fresh block), shrink-in-place, and
// assign-then-construct-the-tail; size becomes the source size, capacity only grows.
struct E { int a, b, c, d; };
struct V { E* s; E* f; E* e; };
static void assign(V& t, const V& x)
{
    if (&x == &t) return;
    unsigned n = x.f - x.s;
    if (n > (unsigned)(t.e - t.s)) {
        E* tmp = n ? (E*)std::malloc(n * sizeof(E)) : 0;
        for (unsigned i = 0; i < n; ++i) tmp[i] = x.s[i];
        if (t.s) std::free(t.s);
        t.s = tmp; t.e = tmp + n;
    } else if ((unsigned)(t.f - t.s) >= n) {
        for (unsigned i = 0; i < n; ++i) t.s[i] = x.s[i];
    } else {
        unsigned sz = t.f - t.s;
        for (unsigned i = 0; i < sz; ++i) t.s[i] = x.s[i];
        for (unsigned i = sz; i < n; ++i) t.s[i] = x.s[i];
    }
    t.f = t.s + n;
}
static V mk(unsigned n, unsigned cap, int base)
{
    V v; v.s = (E*)std::malloc(cap * sizeof(E)); v.f = v.s + n; v.e = v.s + cap;
    for (unsigned i = 0; i < n; ++i) { E x = { base + (int)i, 1, 2, 3 }; v.s[i] = x; }
    return v;
}
int main()
{
    V a = mk(2, 2, 0), b = mk(5, 5, 100);
    assign(a, b);
    if (a.f - a.s != 5 || a.e - a.s != 5 || a.s[4].a != 104) { std::printf("00a81690_TEST FAIL\n"); return 1; }
    V c = mk(3, 8, 200);
    assign(a, c);
    if (a.f - a.s != 3 || a.e - a.s != 5 || a.s[2].a != 202) { std::printf("00a81690_TEST FAIL\n"); return 1; }
    V d = mk(1, 8, 0), e = mk(4, 4, 300);
    assign(d, e);
    if (d.f - d.s != 4 || d.e - d.s != 8 || d.s[3].a != 303 || d.s[0].a != 300) { std::printf("00a81690_TEST FAIL\n"); return 1; }
    assign(d, d);
    if (d.f - d.s != 4) { std::printf("00a81690_TEST FAIL\n"); return 1; }
    std::printf("00a81690_TEST PASS\n");
    return 0;
}