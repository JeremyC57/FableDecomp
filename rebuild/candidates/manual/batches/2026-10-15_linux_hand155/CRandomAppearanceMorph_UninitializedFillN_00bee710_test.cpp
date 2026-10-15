// Behaviour test for 0x00BEE710: uninitialized fill of n records, each a deep copy (4 words + its own copy of the entry vector).
#include <cstdio>
#include <cstdlib>
#include <new>
struct Entry { unsigned int w[4]; };
struct Vec { Entry* s; Entry* f; Entry* e;
  Vec(const Vec& o) { unsigned int n = (unsigned int)(o.f - o.s); s = n ? (Entry*)std::malloc(n * sizeof(Entry)) : 0; f = s; e = s + n; for (Entry* p = o.s; p != o.f; ++p, ++f) new (f) Entry(*p); }
  Vec() : s(0), f(0), e(0) {} };
struct Rec { unsigned int a, b, c, d; Vec v; };
static Rec* Fill(Rec* first, unsigned int n, const Rec& x) { Rec* cur = first; for (; n > 0; --n, ++cur) new (cur) Rec(x); return cur; }
int main()
{
    Entry es[2] = {{{1, 2, 3, 4}}, {{5, 6, 7, 8}}};
    Rec x; x.a = 10; x.b = 11; x.c = 12; x.d = 13; x.v.s = es; x.v.f = es + 2; x.v.e = es + 2;
    void* mem = std::malloc(3 * sizeof(Rec)); Rec* r = (Rec*)mem;
    Rec* end = Fill(r, 3, x);
    bool ok = end == r + 3 && r[2].a == 10 && r[2].d == 13 && r[1].v.f - r[1].v.s == 2 && r[1].v.s != es && r[1].v.s[1].w[3] == 8 && r[0].v.s != r[1].v.s;
    ok = ok && Fill(r, 0, x) == r;
    if (!ok) { std::printf("00bee710_TEST FAIL\n"); return 1; }
    std::printf("00bee710_TEST PASS\n");
    return 0;
}
