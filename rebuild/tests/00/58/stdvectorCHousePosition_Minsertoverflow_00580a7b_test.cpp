// Behaviour test for 0x00580A7B std::vector<pair<const CRegion*, C3DVector>>::_M_insert_overflow (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stddef.h>
struct Rec { unsigned int region; float x, y, z; };
static Rec* ucopy(const Rec* f, const Rec* l, Rec* r) { Rec* c = r; for (; f != l; ++f, ++c) new (c) Rec(*f); return c; }
static Rec* ufill_n(Rec* f, size_t n, const Rec& x) { for (; n > 0; --n, ++f) new (f) Rec(x); return f; }
template <class T> const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
struct Vec {
    Rec* start; Rec* finish; Rec* eos;
    size_t size() const { return size_t(finish - start); }
    void insert_overflow(Rec* pos, const Rec& x, size_t fill_len, bool atend)
    {
        const size_t old = size();
        const size_t len = old + tmax(old, fill_len);
        Rec* ns = len != 0 ? (Rec*)malloc(len * sizeof(Rec)) : 0;
        Rec* nf = ucopy(start, pos, ns);
        if (fill_len == 1) { new (nf) Rec(x); ++nf; } else nf = ufill_n(nf, fill_len, x);
        if (!atend) nf = ucopy(pos, finish, nf);
        if (start) free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
static Rec R(unsigned int k) { Rec r; r.region = k; r.x = (float)k; r.y = 0.0f; r.z = 1.0f; return r; }
int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    v.insert_overflow(v.finish, R(1), 1, true);             // push_back into an empty vector
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].region != 1) { std::printf("00580a7b_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, R(2), 1, true);
    if (v.size() != 2 || v.eos - v.start != 2 || v.start[1].region != 2) { std::printf("00580a7b_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start + 1, R(5), 3, false);         // 1 5 5 5 2, capacity 2 + 3
    unsigned int want[5] = { 1, 5, 5, 5, 2 };
    if (v.size() != 5 || v.eos - v.start != 5) { std::printf("00580a7b_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (v.start[i].region != want[i] || v.start[i].x != (float)want[i]) { std::printf("00580a7b_TEST FAIL\n"); return 1; }
    std::printf("00580a7b_TEST PASS\n");
    return 0;
}