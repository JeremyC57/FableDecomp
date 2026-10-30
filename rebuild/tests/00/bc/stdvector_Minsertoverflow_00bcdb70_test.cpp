// Behaviour test for vector<CArray<C3DVector> >::_M_insert_overflow (0x00BCDB70): growth insert with deep element copies.
#include <cstdio>
#include <cstdlib>
#include <new>
struct Elem {                                  // stands in for CArray<C3DVector>: owns a malloc block
    int* p; int n;
    Elem(int v) : p((int*)std::malloc(sizeof(int))), n(1) { *p = v; }
    Elem(const Elem& o) : p((int*)std::malloc(sizeof(int) * o.n)), n(o.n) { for (int i = 0; i < n; i++) p[i] = o.p[i]; }
    ~Elem() { std::free(p); }
};
struct Vec {
    Elem* s; Elem* f; Elem* e;
    unsigned int size() const { return (unsigned int)(f - s); }
    void clear_() { for (Elem* q = s; q != f; ++q) q->~Elem(); if (s) std::free(s); }
    void insert_overflow(Elem* pos, const Elem& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < fill ? fill : old);
        Elem* ns = len ? (Elem*)std::malloc(len * sizeof(Elem)) : 0;
        Elem* nf = ns;
        for (Elem* q = s; q != pos; ++q, ++nf) new (nf) Elem(*q);
        if (fill == 1) { new (nf) Elem(x); ++nf; }
        else for (unsigned int k = fill; k > 0; --k, ++nf) new (nf) Elem(x);
        if (!atend) for (Elem* q = pos; q != f; ++q, ++nf) new (nf) Elem(*q);
        clear_();
        s = ns; f = nf; e = ns + len;
    }
};
int main()
{
    Vec v; v.s = (Elem*)std::malloc(3 * sizeof(Elem)); v.f = v.s; v.e = v.s + 3;
    for (int i = 0; i < 3; i++) { new (v.f) Elem(10 + i); ++v.f; }
    Elem x(99);
    v.insert_overflow(v.s + 1, x, 1, false);        // 10 99 11 12, capacity 6
    bool ok = v.size() == 4 && v.e - v.s == 6 && *v.s[0].p == 10 && *v.s[1].p == 99 && *v.s[2].p == 11 && *v.s[3].p == 12;
    v.insert_overflow(v.f, x, 5, true);              // append 5 copies: size 9, capacity 4 + 5
    ok = ok && v.size() == 9 && v.e - v.s == 9 && *v.s[8].p == 99 && *v.s[3].p == 12 && v.s[4].p != x.p;
    v.clear_();
    if (!ok) { std::printf("00bcdb70_TEST FAIL\n"); return 1; }
    std::printf("00bcdb70_TEST PASS\n");
    return 0;
}