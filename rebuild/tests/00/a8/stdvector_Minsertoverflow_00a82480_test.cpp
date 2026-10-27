// Behaviour test for 0x00A82480 std::vector<pair<unsigned __int64, X> >::_M_insert_overflow (grow + insert fill_len copies)
#include <cstdio>
#include <cstdlib>

static int g_dummy;
typedef const void* Second;
struct Pair { unsigned long long first; Second second; };

struct Vec {
    Pair* start; Pair* finish; Pair* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(Pair* pos, const Pair& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Pair* ns = len ? (Pair*)std::malloc(len * sizeof(Pair)) : 0;
        Pair* nf = ns;
        for (Pair* p = start; p != pos; ++p, ++nf) *nf = *p;
        if (fill_len == 1) { *nf = x; ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) *nf = x;
        if (!atend) for (Pair* p = pos; p != finish; ++p, ++nf) *nf = *p;
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = (Pair*)std::malloc(3 * sizeof(Pair)); v.finish = v.start + 3; v.eos = v.finish;
    for (int i = 0; i < 3; ++i) { v.start[i].first = 100ull * i + 0x100000000ull; v.start[i].second = Second(); }
    Pair x; x.first = 0xABCDEF0123ull; x.second = (const void*)&g_dummy;
    v.insert_overflow(v.start + 1, x, 1, false);          // push into the middle: capacity doubles
    if (v.size() != 4 || v.eos - v.start != 6 || v.start[1].first != x.first || v.start[1].second != x.second
        || v.start[2].first != 100ull + 0x100000000ull || v.start[3].first != 200ull + 0x100000000ull) { std::printf("00a82480_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.finish, x, 5, true);              // fill 5 at the end: len = 4 + max(4,5) = 9
    if (v.size() != 9 || v.eos - v.start != 9 || v.start[8].first != x.first || v.start[0].first != 0x100000000ull) { std::printf("00a82480_TEST FAIL\n"); return 1; }
    std::printf("00a82480_TEST PASS\n");
    return 0;
}