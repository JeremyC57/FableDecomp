// Behaviour test for 0x00594D36 std::vector<pair<long, CDefPointer<...>>>::_M_insert_overflow (model copy, counts live elements)
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stddef.h>
static int g_live = 0;
struct Def { int refs; };
struct Pair {
    int first; Def* second;
    Pair(int f, Def* d) : first(f), second(d) { ++second->refs; ++g_live; }
    Pair(const Pair& o) : first(o.first), second(o.second) { ++second->refs; ++g_live; }
    ~Pair() { --second->refs; --g_live; }
};
static Pair* ucopy(const Pair* f, const Pair* l, Pair* r) { Pair* c = r; for (; f != l; ++f, ++c) new (c) Pair(*f); return c; }
static Pair* ufill_n(Pair* f, size_t n, const Pair& x) { for (; n > 0; --n, ++f) new (f) Pair(x); return f; }
template <class T> const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
struct Vec {
    Pair* start; Pair* finish; Pair* eos;
    size_t size() const { return size_t(finish - start); }
    void clear_storage() { for (Pair* p = start; p != finish; ++p) p->~Pair(); if (start) free(start); }
    void insert_overflow(Pair* pos, const Pair& x, size_t fill_len, bool atend)
    {
        const size_t old = size();
        const size_t len = old + tmax(old, fill_len);
        Pair* ns = len != 0 ? (Pair*)malloc(len * sizeof(Pair)) : 0;
        Pair* nf = ucopy(start, pos, ns);
        if (fill_len == 1) { new (nf) Pair(x); ++nf; } else nf = ufill_n(nf, fill_len, x);
        if (!atend) nf = ucopy(pos, finish, nf);
        clear_storage();
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    Def d; d.refs = 0;
    Vec v; v.start = v.finish = v.eos = 0;
    { Pair a(1, &d); v.insert_overflow(v.finish, a, 1, true); }
    { Pair b(2, &d); v.insert_overflow(v.finish, b, 1, true); }
    { Pair c(3, &d); v.insert_overflow(v.start + 1, c, 2, false); }   // 1 3 3 2
    int want[4] = { 1, 3, 3, 2 };
    if (v.size() != 4 || v.eos - v.start != 4 || g_live != 4 || d.refs != 4) { std::printf("00594d36_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) if (v.start[i].first != want[i]) { std::printf("00594d36_TEST FAIL\n"); return 1; }
    v.clear_storage();
    if (g_live != 0 || d.refs != 0) { std::printf("00594d36_TEST FAIL\n"); return 1; }
    std::printf("00594d36_TEST PASS\n");
    return 0;
}