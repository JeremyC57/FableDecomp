// Behaviour test for 0x00519CD0 vector<CMapInfo>::_M_range_insert (STLport insert(pos, first, last)).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live = 0;
struct CMapInfo {
    unsigned int MapUID;
    CMapInfo(unsigned int id) : MapUID(id) { ++g_live; }
    CMapInfo(const CMapInfo& o) : MapUID(o.MapUID) { ++g_live; }
    CMapInfo& operator=(const CMapInfo& o) { MapUID = o.MapUID; return *this; }
    ~CMapInfo() { --g_live; }
};

struct Vec {
    CMapInfo* start; CMapInfo* finish; CMapInfo* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    static CMapInfo* ucopy(const CMapInfo* f, const CMapInfo* l, CMapInfo* r) { for (; f != l; ++f, ++r) new (r) CMapInfo(*f); return r; }
    void range_insert(CMapInfo* pos, const CMapInfo* first, const CMapInfo* last)
    {
        if (first == last) return;
        unsigned int n = (unsigned int)(last - first);
        if ((unsigned int)(eos - finish) >= n) {
            unsigned int after = (unsigned int)(finish - pos);
            CMapInfo* old_finish = finish;
            if (after > n) {
                ucopy(finish - n, finish, finish);
                finish += n;
                CMapInfo* r = old_finish; CMapInfo* l = old_finish - n;
                for (int k = (int)(l - pos); k > 0; --k) *--r = *--l;
                CMapInfo* o = pos;
                for (int k = (int)(last - first); k > 0; --k) *o++ = *first++;
            } else {
                const CMapInfo* mid = first + after;
                ucopy(mid, last, finish);
                finish += n - after;
                ucopy(pos, old_finish, finish);
                finish += after;
                CMapInfo* o = pos;
                for (int k = (int)(mid - first); k > 0; --k) *o++ = *first++;
            }
        } else {
            unsigned int old_size = size();
            unsigned int len = old_size + (old_size < n ? n : old_size);
            CMapInfo* ns = len ? (CMapInfo*)std::malloc(len * sizeof(CMapInfo)) : 0;
            CMapInfo* nf = ucopy(start, pos, ns);
            nf = ucopy(first, last, nf);
            nf = ucopy(pos, finish, nf);
            for (CMapInfo* p = start; p != finish; ++p) p->~CMapInfo();
            if (start) std::free(start);
            start = ns; finish = nf; eos = ns + len;
        }
    }
};

static int check(const Vec& v, const unsigned int* w, unsigned int n)
{
    if (v.size() != n) return 0;
    for (unsigned int i = 0; i < n; ++i) if (v.start[i].MapUID != w[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    {
        CMapInfo src[4] = { CMapInfo(1), CMapInfo(2), CMapInfo(3), CMapInfo(4) };
        Vec v; v.start = v.finish = v.eos = 0;
        v.range_insert(v.finish, src, src + 2);           // grow: [1 2] cap 2
        v.range_insert(v.start + 1, src + 2, src + 4);    // grow: [1 3 4 2] cap 4
        { unsigned int w[] = { 1, 3, 4, 2 }; if (!check(v, w, 4) || v.eos - v.start != 4) ok = 0; }
        v.range_insert(v.start, src, src);                // empty range: no-op
        v.range_insert(v.start + 2, src + 3, src + 4);    // grow: [1 3 4 4 2] cap 8
        { unsigned int w[] = { 1, 3, 4, 4, 2 }; if (!check(v, w, 5) || v.eos - v.start != 8) ok = 0; }
        v.range_insert(v.start + 1, src, src + 1);        // in place, after (4) > n (1): [1 1 3 4 4 2]
        { unsigned int w[] = { 1, 1, 3, 4, 4, 2 }; if (!check(v, w, 6)) ok = 0; }
        v.range_insert(v.start + 5, src + 1, src + 3);    // in place, after (1) <= n (2): [1 1 3 4 4 2 3 2]
        { unsigned int w[] = { 1, 1, 3, 4, 4, 2, 3, 2 }; if (!check(v, w, 8)) ok = 0; }
        if (g_live != 4 + 8) ok = 0;
        for (CMapInfo* p = v.start; p != v.finish; ++p) p->~CMapInfo();
        std::free(v.start);
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00519cd0_TEST FAIL\n"); return 1; }
    std::printf("00519cd0_TEST PASS\n");
    return 0;
}