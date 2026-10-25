// Behaviour test for 0x00728D10: _M_insert_overflow with a non-trivial element (copies own storage, old range destroyed)
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;

struct Part {
    unsigned long MeshIndex;
    long*         morphs;                                   // owned copy of one value, stands in for the inner array
    Part(unsigned long k, long v) : MeshIndex(k), morphs((long*)std::malloc(sizeof(long))) { *morphs = v; ++g_live; }
    Part(const Part& o) : MeshIndex(o.MeshIndex), morphs((long*)std::malloc(sizeof(long))) { *morphs = *o.morphs; ++g_live; }
    ~Part() { std::free(morphs); --g_live; }
};

struct Vec {
    Part* start;
    Part* finish;
    Part* end_of_storage;
    void insert_overflow(Part* pos, const Part& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = (unsigned int)(finish - start);
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        Part* ns = len != 0 ? (Part*)std::malloc(len * sizeof(Part)) : 0;
        Part* nf = ns;
        for (Part* p = start; p != pos; ++p, ++nf) new (nf) Part(*p);
        for (unsigned int i = 0; i < fill_len; ++i, ++nf) new (nf) Part(x);
        if (!atend)
            for (Part* p = pos; p != finish; ++p, ++nf) new (nf) Part(*p);
        for (Part* p = start; p != finish; ++p) p->~Part();
        if (start != 0) std::free(start);
        start = ns; finish = nf; end_of_storage = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.end_of_storage = 0;
    { Part a(1, 10); v.insert_overflow(v.finish, a, 1, true); }
    { Part b(2, 20); v.insert_overflow(v.finish, b, 1, true); }
    if (g_live != 2 || v.finish - v.start != 2 || v.end_of_storage - v.start != 2) { std::printf("00728d10_TEST FAIL\n"); return 1; }
    { Part c(7, 70); v.insert_overflow(v.start + 1, c, 2, false); }
    if (g_live != 4 || v.finish - v.start != 4 || v.end_of_storage - v.start != 4) { std::printf("00728d10_TEST FAIL\n"); return 1; }
    unsigned long wk[4] = { 1, 7, 7, 2 };
    long wv[4] = { 10, 70, 70, 20 };
    for (int i = 0; i < 4; ++i)
        if (v.start[i].MeshIndex != wk[i] || *v.start[i].morphs != wv[i]) { std::printf("00728d10_TEST FAIL\n"); return 1; }
    for (Part* p = v.start; p != v.finish; ++p) p->~Part();
    std::free(v.start);
    if (g_live != 0) { std::printf("00728d10_TEST FAIL\n"); return 1; }
    std::printf("00728d10_TEST PASS\n");
    return 0;
}