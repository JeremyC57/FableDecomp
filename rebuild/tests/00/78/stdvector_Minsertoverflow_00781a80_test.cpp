// Behaviour test for 0x00781A80 std::vector<NFire::CFirePoint>::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0;                          // live fire points

struct CFirePoint {
    char  HelperName[8];
    long  HelperIndex;
    long  EffectIndex;
    long  Resistance;
    bool  Burning;
    long  FireDamageAfterFrame;
    void* PParentThing;
    void* PEffect;
    void* PSingeEffect;
    CFirePoint(const char* n, long idx) : HelperIndex(idx), EffectIndex(idx * 2), Resistance(3), Burning(false),
        FireDamageAfterFrame(0), PParentThing(0), PEffect(0), PSingeEffect(0) { std::strcpy(HelperName, n); ++g_live; }
    CFirePoint(const CFirePoint& o) : HelperIndex(o.HelperIndex), EffectIndex(o.EffectIndex), Resistance(o.Resistance),
        Burning(o.Burning), FireDamageAfterFrame(o.FireDamageAfterFrame), PParentThing(o.PParentThing),
        PEffect(o.PEffect), PSingeEffect(o.PSingeEffect) { std::strcpy(HelperName, o.HelperName); ++g_live; }
    ~CFirePoint() { --g_live; }
};

struct __false_type { __false_type() { } };

struct Vec {
    CFirePoint* m_start; CFirePoint* m_finish; CFirePoint* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(CFirePoint* pos, const CFirePoint& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CFirePoint* ns = len != 0 ? (CFirePoint*)std::malloc(len * sizeof(CFirePoint)) : 0;
        CFirePoint* nf = ns;
        for (CFirePoint* p = m_start; p != pos; ++p, ++nf) new (nf) CFirePoint(*p);
        if (fill_len == 1) { new (nf) CFirePoint(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CFirePoint(x);
        if (!atend) for (CFirePoint* p = pos; p != m_finish; ++p, ++nf) new (nf) CFirePoint(*p);
        for (CFirePoint* d = m_start; d != m_finish; ++d) d->~CFirePoint();      // __destroy_aux 0x007817B0
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

int main()
{
    int ok = 1;
    {
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        CFirePoint a("hand", 1), b("head", 2), c("foot", 3);
        v.insert_overflow(v.m_finish, a, __false_type(), 1, true);
        v.insert_overflow(v.m_finish, b, __false_type(), 1, true);
        v.insert_overflow(v.m_start + 1, c, __false_type(), 2, false);  // len = 2 + 2 = 4
        if (v.size() != 4 || v.m_end - v.m_start != 4) ok = 0;
        if (v.m_start[0].HelperIndex != 1 || v.m_start[1].EffectIndex != 6 || v.m_start[2].HelperIndex != 3) ok = 0;
        if (std::strcmp(v.m_start[3].HelperName, "head") != 0) ok = 0;
        if (g_live != 3 + 4) ok = 0;
        for (CFirePoint* d = v.m_start; d != v.m_finish; ++d) d->~CFirePoint();
        std::free(v.m_start);
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("00781a80_TEST FAIL\n"); return 1; }
    std::printf("00781a80_TEST PASS\n");
    return 0;
}