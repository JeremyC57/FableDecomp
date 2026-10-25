// Behaviour test for 0x0080EF50 vector<pair<CIntelligentPointer<CThing>,long> >::insert(iterator, const value_type&).
#include <cstdio>
#include <cstdlib>
struct CObj { int id; };
struct CPtr {
    CObj* PData;
    CPtr() : PData(0) { }
    CPtr(const CPtr& o) : PData(o.PData) { }
    CPtr& operator=(const CPtr& o) { PData = o.PData; return *this; }
};
struct CEntry { CPtr first; long second; };
inline void* operator new(size_t, void* p, int) { return p; }
static int g_overflow = 0;
struct CVec {
    CEntry* start; CEntry* finish; CEntry* eos;
    void insert_overflow(CEntry* pos, const CEntry& x)
    {
        ++g_overflow;
        unsigned int old = (unsigned int)(finish - start);
        unsigned int len = old + (old < 1 ? 1 : old);
        CEntry* ns = (CEntry*)std::malloc(len * sizeof(CEntry));
        CEntry* nf = ns;
        for (CEntry* p = start; p != pos; ++p, ++nf) new (nf, 0) CEntry(*p);
        new (nf, 0) CEntry(x); ++nf;
        for (CEntry* p = pos; p != finish; ++p, ++nf) new (nf, 0) CEntry(*p);
        std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
    CEntry* insert(CEntry* pos, const CEntry& x)
    {
        unsigned int n = (unsigned int)(pos - start);
        if (finish != eos) {
            if (pos == finish) { new (finish, 0) CEntry(x); ++finish; }
            else {
                new (finish, 0) CEntry(*(finish - 1));
                ++finish;
                CEntry x_copy = x;
                CEntry* last = finish - 2; CEntry* result = finish - 1;
                for (int k = (int)(last - pos); k > 0; --k) *--result = *--last;
                *pos = x_copy;
            }
        } else
            insert_overflow(pos, x);
        return start + n;
    }
};
int main()
{
    CObj o[6];
    CVec v; v.start = (CEntry*)std::malloc(4 * sizeof(CEntry)); v.finish = v.start; v.eos = v.start + 4;
    int bad = 0;
    for (int i = 0; i < 3; ++i) { CEntry e; e.first.PData = &o[i]; e.second = i; CEntry* r = v.insert(v.finish, e); if (r != v.start + i) bad = 1; }
    CEntry m; m.first.PData = &o[4]; m.second = 40;
    CEntry* r = v.insert(v.start + 1, m);                       // room: shift tail, 0 4 1 2
    if (r != v.start + 1 || v.finish - v.start != 4) bad = 1;
    if (v.start[1].first.PData != &o[4] || v.start[2].second != 1 || v.start[3].first.PData != &o[2]) bad = 1;
    CEntry z; z.first.PData = &o[5]; z.second = 50;
    r = v.insert(v.start, z);                                    // full: overflow, 5 0 4 1 2
    if (g_overflow != 1 || r != v.start || v.finish - v.start != 5 || v.start[0].second != 50 || v.start[4].second != 2) bad = 1;
    std::free(v.start);
    if (bad) { std::printf("0080ef50_TEST FAIL\n"); return 1; }
    std::printf("0080ef50_TEST PASS\n");
    return 0;
}