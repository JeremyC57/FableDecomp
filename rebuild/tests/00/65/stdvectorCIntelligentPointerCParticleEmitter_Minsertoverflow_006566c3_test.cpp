// Behaviour test for 0x006566C3 vector<CIntelligentPointer<CParticleEmitter>>::_M_insert_overflow (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Ptr {
    void* item;
    Ptr(void* p) : item(p) { ++g_live; }
    Ptr(const Ptr& o) : item(o.item) { ++g_live; }
    ~Ptr() { --g_live; }
};
template <class T> const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
Ptr* ucopy(Ptr* f, Ptr* l, Ptr* r) { for (; f != l; ++f, ++r) new (r) Ptr(*f); return r; }
Ptr* ufill(Ptr* f, unsigned int n, const Ptr& x) { for (; n > 0; --n, ++f) new (f) Ptr(x); return f; }
struct Vec {
    Ptr* start; Ptr* finish; Ptr* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void clear_() { for (Ptr* p = start; p != finish; ++p) p->~Ptr(); if (start) free(start); }
    void insert_overflow(Ptr* pos, const Ptr& x, unsigned int n, bool atend)
    {
        const unsigned int old = size();
        const unsigned int len = old + tmax(old, n);
        Ptr* ns = len != 0 ? (Ptr*)malloc(len * sizeof(Ptr)) : 0;
        Ptr* nf = ucopy(start, pos, ns);
        if (n == 1) { new (nf) Ptr(x); ++nf; } else nf = ufill(nf, n, x);
        if (!atend) nf = ucopy(pos, finish, nf);
        clear_();
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    int a, b, c;
    Vec v; v.start = (Ptr*)malloc(2 * sizeof(Ptr)); new (v.start) Ptr(&a); new (v.start + 1) Ptr(&b); v.finish = v.eos = v.start + 2;
    Ptr x(&c);
    v.insert_overflow(v.start + 1, x, 1, false);          // a c b, capacity 4
    if (v.size() != 3 || v.eos - v.start != 4 || v.start[0].item != &a || v.start[1].item != &c || v.start[2].item != &b || g_live != 4)
        { std::printf("006566c3_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, x, 5, true);              // a c b c c c c c, capacity 3 + 5 = 8
    if (v.size() != 8 || v.eos - v.start != 8 || v.start[7].item != &c || v.start[2].item != &b || g_live != 9)
        { std::printf("006566c3_TEST FAIL\n"); return 1; }
    std::printf("006566c3_TEST PASS\n");
    return 0;
}