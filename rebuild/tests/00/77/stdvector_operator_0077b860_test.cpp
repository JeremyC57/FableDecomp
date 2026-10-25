// Behaviour test for 0x0077B860 std::vector<CEngineTextureDecal>::operator= (STLport vector assignment, the element
// carries its own vector of 16-byte blend descriptions).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_Blocks = 0;

struct CCompositeBlendDesc { int a, b, c, d; };

struct BlendVec {
    CCompositeBlendDesc* s;
    CCompositeBlendDesc* f;
    CCompositeBlendDesc* e;
    BlendVec() : s(0), f(0), e(0) { }
    BlendVec(const BlendVec& o) : s(0), f(0), e(0) { assign(o); }
    BlendVec& operator=(const BlendVec& o) { if (&o != this) assign(o); return *this; }
    ~BlendVec() { release(); }
    unsigned int size() const { return (unsigned int)(f - s); }
    void release() { if (s) { std::free(s); --g_Blocks; } s = f = e = 0; }
    void assign(const BlendVec& o)
    {
        release();
        unsigned int n = o.size();
        if (n) {
            s = (CCompositeBlendDesc*)std::malloc(n * sizeof(CCompositeBlendDesc));
            ++g_Blocks;
            std::memcpy(s, o.s, n * sizeof(CCompositeBlendDesc));
        }
        f = e = s + n;
    }
    void push(int v)
    {
        unsigned int n = size();
        CCompositeBlendDesc* ns = (CCompositeBlendDesc*)std::malloc((n + 1) * sizeof(CCompositeBlendDesc));
        ++g_Blocks;
        if (n) std::memcpy(ns, s, n * sizeof(CCompositeBlendDesc));
        ns[n].a = v; ns[n].b = v + 1; ns[n].c = v + 2; ns[n].d = v + 3;
        release();
        s = ns; f = e = ns + n + 1;
    }
};

struct CEngineTextureDecal {
    int      OriginalTextureIndex;
    int      DecalID;
    BlendVec OverlaidBlends;
};

struct Vec {
    CEngineTextureDecal* m_start;
    CEngineTextureDecal* m_finish;
    CEngineTextureDecal* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    unsigned int capacity() const { return (unsigned int)(m_end - m_start); }
    static CEngineTextureDecal* ucopy(const CEngineTextureDecal* f, const CEngineTextureDecal* l, CEngineTextureDecal* r)
    {
        for (; f != l; ++f, ++r) new (r) CEngineTextureDecal(*f);
        return r;
    }
    static CEngineTextureDecal* acopy(const CEngineTextureDecal* f, const CEngineTextureDecal* l, CEngineTextureDecal* r)
    {
        for (int n = (int)(l - f); n > 0; --n, ++f, ++r) *r = *f;
        return r;
    }
    static void destroy(CEngineTextureDecal* f, CEngineTextureDecal* l) { for (; f != l; ++f) f->~CEngineTextureDecal(); }
    void clear() { destroy(m_start, m_finish); if (m_start) std::free(m_start); }
    Vec& assign(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CEngineTextureDecal* tmp = xlen != 0 ? (CEngineTextureDecal*)std::malloc(xlen * sizeof(CEngineTextureDecal)) : 0;
                ucopy(x.m_start, x.m_finish, tmp);
                clear();
                m_start = tmp;
                m_end = m_start + xlen;
            } else if (size() >= xlen) {
                CEngineTextureDecal* i = acopy(x.m_start, x.m_finish, m_start);
                destroy(i, m_finish);
            } else {
                acopy(x.m_start, x.m_start + size(), m_start);
                ucopy(x.m_start + size(), x.m_finish, m_finish);
            }
            m_finish = m_start + xlen;
        }
        return *this;
    }
};

static void make(Vec& v, unsigned int cap, unsigned int n, int base)
{
    v.m_start = (CEngineTextureDecal*)std::malloc(cap * sizeof(CEngineTextureDecal));
    v.m_finish = v.m_start;
    v.m_end = v.m_start + cap;
    for (unsigned int i = 0; i < n; ++i, ++v.m_finish) {
        new (v.m_finish) CEngineTextureDecal();
        v.m_finish->OriginalTextureIndex = base + (int)i;
        v.m_finish->DecalID = base + 50 + (int)i;
        for (unsigned int k = 0; k <= i; ++k) v.m_finish->OverlaidBlends.push(base * 10 + (int)k);
    }
}

int main()
{
    int ok = 1;
    Vec a, b, c, d;
    make(a, 3, 3, 10);                                      // blends per element: 1, 2, 3
    make(b, 1, 1, 40);
    b.assign(a);                                            // grow: reallocate to exactly 3
    if (b.size() != 3 || b.capacity() != 3 || b.m_start[2].DecalID != 62 || b.m_start[2].OverlaidBlends.size() != 3 ||
        b.m_start[2].OverlaidBlends.s[2].d != 105)
        ok = 0;
    make(c, 4, 4, 20);
    c.assign(a);                                            // shrink in place: 3 assigned, 1 destroyed
    if (c.size() != 3 || c.capacity() != 4 || c.m_start[0].OriginalTextureIndex != 10 ||
        c.m_start[1].OverlaidBlends.size() != 2 || c.m_start[1].OverlaidBlends.s[1].a != 101)
        ok = 0;
    make(d, 5, 1, 30);
    d.assign(a);                                            // partial: 1 assigned, 2 copy-constructed
    if (d.size() != 3 || d.capacity() != 5 || d.m_start[0].OverlaidBlends.size() != 1 ||
        d.m_start[0].OverlaidBlends.s[0].b != 101 || d.m_start[2].DecalID != 62)
        ok = 0;
    if (g_Blocks != 4 * 3) ok = 0;                          // each of a, b, c, d holds 3 blend blocks
    a.clear(); b.clear(); c.clear(); d.clear();
    if (g_Blocks != 0) ok = 0;
    if (!ok) { std::printf("0077b860_TEST FAIL\n"); return 1; }
    std::printf("0077b860_TEST PASS\n");
    return 0;
}