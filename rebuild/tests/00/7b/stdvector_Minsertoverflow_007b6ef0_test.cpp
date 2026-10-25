// Behaviour test for 0x007B6EF0 std::vector<CIntelligentPointer<CThingMarker> >::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_refs = 0;                          // live pointer registrations

struct CBaseIntelligentPointer {
    void* PData;
    CBaseIntelligentPointer() : PData(0) { }
    virtual ~CBaseIntelligentPointer() { if (PData) --g_refs; }
    void* GetPItem() const { return PData; }
    void SetPItem(void* item) { if (PData) --g_refs; PData = item; if (PData) ++g_refs; }
};
struct CMarkerPointer : CBaseIntelligentPointer {
    CMarkerPointer(void* p) { SetPItem(p); }
    CMarkerPointer(const CMarkerPointer& o) : CBaseIntelligentPointer() { SetPItem(o.GetPItem()); }
};

struct __false_type { __false_type() { } };

struct Vec {
    CMarkerPointer* m_start; CMarkerPointer* m_finish; CMarkerPointer* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(CMarkerPointer* pos, const CMarkerPointer& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CMarkerPointer* ns = len != 0 ? (CMarkerPointer*)std::malloc(len * sizeof(CMarkerPointer)) : 0;
        CMarkerPointer* nf = ns;
        for (CMarkerPointer* p = m_start; p != pos; ++p, ++nf) new (nf) CMarkerPointer(*p);
        if (fill_len == 1) { new (nf) CMarkerPointer(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CMarkerPointer(x);
        if (!atend) for (CMarkerPointer* p = pos; p != m_finish; ++p, ++nf) new (nf) CMarkerPointer(*p);
        for (CMarkerPointer* d = m_start; d != m_finish; ++d) d->~CMarkerPointer();
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

int main()
{
    int ok = 1;
    int things[3];
    {
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        CMarkerPointer a(&things[0]), b(&things[1]), c(&things[2]);
        v.insert_overflow(v.m_finish, a, __false_type(), 1, true);
        v.insert_overflow(v.m_finish, b, __false_type(), 1, true);
        v.insert_overflow(v.m_finish, c, __false_type(), 1, true);       // len = 2 + 2 = 4
        if (v.size() != 3 || v.m_end - v.m_start != 4) ok = 0;
        v.insert_overflow(v.m_start, c, __false_type(), 3, false);      // len = 3 + 3 = 6
        if (v.size() != 6 || v.m_end - v.m_start != 6) ok = 0;
        if (v.m_start[0].GetPItem() != &things[2] || v.m_start[2].GetPItem() != &things[2] ||
            v.m_start[3].GetPItem() != &things[0] || v.m_start[5].GetPItem() != &things[2]) ok = 0;
        if (g_refs != 3 + 6) ok = 0;
        for (CMarkerPointer* d = v.m_start; d != v.m_finish; ++d) d->~CMarkerPointer();
        std::free(v.m_start);
    }
    if (g_refs != 0) ok = 0;
    if (!ok) { std::printf("007b6ef0_TEST FAIL\n"); return 1; }
    std::printf("007b6ef0_TEST PASS\n");
    return 0;
}