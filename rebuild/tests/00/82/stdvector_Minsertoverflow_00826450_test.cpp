// Behaviour test for 0x00826450 vector<CMap::CScriptedMapBrushUpdateArea>::_M_insert_overflow (STLport growth).
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_live = 0;
struct CCharString {
    char* s;
    CCharString(const char* t) { s = (char*)std::malloc(std::strlen(t) + 1); std::strcpy(s, t); ++g_live; }
    CCharString(const CCharString& o) { s = (char*)std::malloc(std::strlen(o.s) + 1); std::strcpy(s, o.s); ++g_live; }
    ~CCharString() { std::free(s); --g_live; }
};
struct CArea { float Timer; CCharString Name; CArea(float t, const char* n) : Timer(t), Name(n) {} };
inline void* operator new(size_t, void* p, int) { return p; }
struct CVec {
    CArea* start; CArea* finish; CArea* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CArea* pos, const CArea& x, unsigned int fill, bool atend)
    {
        unsigned int old = size();
        unsigned int len = old + (old < fill ? fill : old);
        CArea* ns = len ? (CArea*)std::malloc(len * sizeof(CArea)) : 0;
        CArea* nf = ns;
        for (CArea* p = start; p != pos; ++p, ++nf) new (nf, 0) CArea(*p);
        if (fill == 1) { new (nf, 0) CArea(x); ++nf; }
        else for (unsigned int n = fill; n > 0; --n, ++nf) new (nf, 0) CArea(x);
        if (!atend) for (CArea* p = pos; p != finish; ++p, ++nf) new (nf, 0) CArea(*p);
        for (CArea* p = start; p != finish; ++p) p->~CArea();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    CVec v; v.start = v.finish = v.eos = 0;
    CArea a(1.0f, "alpha"), b(2.0f, "beta"), c(3.0f, "gamma");
    v.insert_overflow(v.finish, a, 1, true);                 // size 1, cap 1
    v.insert_overflow(v.finish, b, 1, true);                 // size 2, cap 2
    v.insert_overflow(v.start + 1, c, 3, false);             // a c c c b, cap 5
    int bad = 0;
    if (v.size() != 5 || v.eos - v.start != 5) bad = 1;
    if (std::strcmp(v.start[0].Name.s, "alpha") || std::strcmp(v.start[1].Name.s, "gamma") ||
        std::strcmp(v.start[3].Name.s, "gamma") || std::strcmp(v.start[4].Name.s, "beta")) bad = 1;
    if (v.start[4].Timer != 2.0f) bad = 1;
    if (g_live != 3 + 5) bad = 1;
    for (CArea* p = v.start; p != v.finish; ++p) p->~CArea();
    std::free(v.start);
    if (bad) { std::printf("00826450_TEST FAIL\n"); return 1; }
    std::printf("00826450_TEST PASS\n");
    return 0;
}