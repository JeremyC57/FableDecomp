// Behaviour test for 0x0081C360 vector<CParticleMorphs::CEntry>::_M_insert_overflow (STLport _M_insert_overflow, 36-byte element).
#include <cstdio>
#include <cstdlib>
static int g_live = 0;
struct CElem {
    int Tag;
    unsigned char _rest[32];
    CElem(int t) : Tag(t) { ++g_live; }
    CElem(const CElem& o) : Tag(o.Tag) { ++g_live; }
    ~CElem() { --g_live; }
};
inline void* operator new(size_t, void* p, int) { return p; }
struct CVec {
    CElem* start; CElem* finish; CElem* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CElem* pos, const CElem& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < fill ? fill : old);
        CElem* ns = len ? (CElem*)std::malloc(len * sizeof(CElem)) : 0;
        CElem* nf = ns;
        for (CElem* p = start; p != pos; ++p, ++nf) new (nf, 0) CElem(*p);
        if (fill == 1) { new (nf, 0) CElem(x); ++nf; }
        else for (unsigned int n = fill; n > 0; --n, ++nf) new (nf, 0) CElem(x);
        if (!atend) for (CElem* p = pos; p != finish; ++p, ++nf) new (nf, 0) CElem(*p);
        for (CElem* p = start; p != finish; ++p) p->~CElem();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    int bad = 0;
    if (sizeof(CElem) != 36) bad = 1;
    CVec v; v.start = v.finish = v.eos = 0;
    CElem a(1), b(2), c(3);
    v.insert_overflow(v.finish, a, 1, true);
    v.insert_overflow(v.finish, b, 1, true);
    v.insert_overflow(v.start + 1, c, 3, false);             // a c c c b, capacity 5
    if (v.size() != 5 || v.eos - v.start != 5) bad = 1;
    if (v.start[0].Tag != 1 || v.start[1].Tag != 3 || v.start[3].Tag != 3 || v.start[4].Tag != 2) bad = 1;
    if (g_live != 3 + 5) bad = 1;
    for (CElem* p = v.start; p != v.finish; ++p) p->~CElem();
    std::free(v.start);
    if (bad) { std::printf("0081c360_TEST FAIL\n"); return 1; }
    std::printf("0081c360_TEST PASS\n");
    return 0;
}