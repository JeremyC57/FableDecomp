// Behaviour test for 0x004D1940 vector<pair<float, CCharString> >::_M_insert_overflow.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CCharString {
    int id;
    CCharString(int v) : id(v) { ++g_live; }
    CCharString(const CCharString& o) : id(o.id) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct Pair {
    float first;
    CCharString second;
    Pair(float f, int s) : first(f), second(s) { }
    Pair(const Pair& o) : first(o.first), second(o.second) { }
};
struct Vec {
    Pair* start; Pair* finish; Pair* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Pair* pos, const Pair& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Pair* ns = len ? (Pair*)std::malloc(len * sizeof(Pair)) : 0;
        Pair* nf = ns;
        for (Pair* p = start; p != pos; ++p, ++nf) new (nf) Pair(*p);
        if (fill_len == 1) { new (nf) Pair(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) Pair(x);
        if (!atend) for (Pair* p = pos; p != finish; ++p, ++nf) new (nf) Pair(*p);
        for (Pair* p = start; p != finish; ++p) p->~Pair();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    int ok = 1;
    Vec v; v.start = v.finish = v.eos = 0;
    Pair a(0.5f, 1), b(1.5f, 2), c(2.5f, 3);
    v.insert_overflow(v.finish, a, 1, true);
    v.insert_overflow(v.finish, c, 1, true);
    v.insert_overflow(v.start + 1, b, 1, false);          // len = 2 + 2 = 4
    if (v.size() != 3 || v.eos - v.start != 4) ok = 0;
    float f[3] = { 0.5f, 1.5f, 2.5f };
    for (int i = 0; i < 3; ++i) {
        float d = v.start[i].first - f[i];
        if (d < -0.001f || d > 0.001f || v.start[i].second.id != i + 1) ok = 0;
    }
    if (g_live != 3 + 3) ok = 0;
    for (Pair* p = v.start; p != v.finish; ++p) p->~Pair();
    std::free(v.start);
    if (g_live != 3) ok = 0;
    if (!ok) { std::printf("004d1940_TEST FAIL\n"); return 1; }
    std::printf("004d1940_TEST PASS\n");
    return 0;
}