// Behaviour test for 0x0082CD80 vector<CMinimapData>::_M_insert_overflow (STLport growth, 60-byte element).
#include <cstdio>
#include <cstdlib>
static int g_live = 0;
class CMinimapData { public:
    int Tag;
    unsigned char _rest[0x38];
    CMinimapData(int t) : Tag(t) { ++g_live; }
    CMinimapData(const CMinimapData& o) : Tag(o.Tag) { ++g_live; }
    ~CMinimapData() { --g_live; }
};
inline void* operator new(size_t, void* p, int) { return p; }
struct CVec {
    CMinimapData* start; CMinimapData* finish; CMinimapData* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CMinimapData* pos, const CMinimapData& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < fill ? fill : old);
        CMinimapData* ns = len ? (CMinimapData*)std::malloc(len * sizeof(CMinimapData)) : 0;
        CMinimapData* nf = ns;
        for (CMinimapData* p = start; p != pos; ++p, ++nf) new (nf, 0) CMinimapData(*p);
        if (fill == 1) { new (nf, 0) CMinimapData(x); ++nf; }
        else for (unsigned int n = fill; n > 0; --n, ++nf) new (nf, 0) CMinimapData(x);
        if (!atend) for (CMinimapData* p = pos; p != finish; ++p, ++nf) new (nf, 0) CMinimapData(*p);
        for (CMinimapData* p = start; p != finish; ++p) p->~CMinimapData();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    int bad = 0;
    if (sizeof(CMinimapData) != 0x3c) bad = 1;
    CVec v; v.start = v.finish = v.eos = 0;
    CMinimapData a(1), b(2), c(3);
    v.insert_overflow(v.finish, a, 1, true);
    v.insert_overflow(v.finish, b, 1, true);
    v.insert_overflow(v.start + 1, c, 2, false);             // a c c b, capacity 4
    if (v.size() != 4 || v.eos - v.start != 4) bad = 1;
    if (v.start[0].Tag != 1 || v.start[1].Tag != 3 || v.start[2].Tag != 3 || v.start[3].Tag != 2) bad = 1;
    if (g_live != 3 + 4) bad = 1;
    for (CMinimapData* p = v.start; p != v.finish; ++p) p->~CMinimapData();
    std::free(v.start);
    if (bad) { std::printf("0082cd80_TEST FAIL\n"); return 1; }
    std::printf("0082cd80_TEST PASS\n");
    return 0;
}