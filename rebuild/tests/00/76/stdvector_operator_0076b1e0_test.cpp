// Behaviour test for 0x0076B1E0 std::vector<ParticleAndEndPoints>::operator= (STLport vector assignment).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_Live = 0;

struct Str {                                                // stand-in for CCharString (4 bytes in retail)
    int v;
    Str() : v(0) { ++g_Live; }
    Str(int x) : v(x) { ++g_Live; }
    Str(const Str& o) : v(o.v) { ++g_Live; }
    Str& operator=(const Str& o) { v = o.v; return *this; }
    ~Str() { --g_Live; }
};

struct ParticleAndEndPoints {
    Str  ParticleName;
    Str  HelperName;
    int  StartPoint;
    int  EndPoint;
    int  PeriodicFrames;
};

struct Vec {
    ParticleAndEndPoints* m_start;
    ParticleAndEndPoints* m_finish;
    ParticleAndEndPoints* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    unsigned int capacity() const { return (unsigned int)(m_end - m_start); }
    static ParticleAndEndPoints* ucopy(const ParticleAndEndPoints* f, const ParticleAndEndPoints* l,
                                       ParticleAndEndPoints* r)
    {
        for (; f != l; ++f, ++r) new (r) ParticleAndEndPoints(*f);
        return r;
    }
    static ParticleAndEndPoints* acopy(const ParticleAndEndPoints* f, const ParticleAndEndPoints* l,
                                       ParticleAndEndPoints* r)
    {
        for (int n = (int)(l - f); n > 0; --n, ++f, ++r) *r = *f;
        return r;
    }
    static void destroy(ParticleAndEndPoints* f, ParticleAndEndPoints* l) { for (; f != l; ++f) f->~ParticleAndEndPoints(); }
    void clear() { destroy(m_start, m_finish); if (m_start) std::free(m_start); }
    Vec& assign(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                ParticleAndEndPoints* tmp = xlen != 0 ? (ParticleAndEndPoints*)std::malloc(xlen * sizeof(ParticleAndEndPoints)) : 0;
                ucopy(x.m_start, x.m_finish, tmp);
                clear();
                m_start = tmp;
                m_end = m_start + xlen;
            } else if (size() >= xlen) {
                ParticleAndEndPoints* i = acopy(x.m_start, x.m_finish, m_start);
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

static void fill(Vec& v, unsigned int cap, unsigned int n, int base)
{
    v.m_start = (ParticleAndEndPoints*)std::malloc(cap * sizeof(ParticleAndEndPoints));
    v.m_finish = v.m_start;
    v.m_end = v.m_start + cap;
    for (unsigned int i = 0; i < n; ++i, ++v.m_finish) {
        new (v.m_finish) ParticleAndEndPoints();
        v.m_finish->ParticleName.v = base + (int)i;
        v.m_finish->HelperName.v = base + 100 + (int)i;
        v.m_finish->StartPoint = (int)i;
        v.m_finish->EndPoint = (int)(i + 1);
        v.m_finish->PeriodicFrames = base + 7;
    }
}

int main()
{
    int ok = 1;
    Vec a, b, c, d;
    fill(a, 3, 3, 10);
    fill(b, 1, 1, 50);
    b.assign(a);                                            // grow: reallocate to exactly 3
    if (b.size() != 3 || b.capacity() != 3 || b.m_start[2].ParticleName.v != 12 || b.m_start[1].HelperName.v != 111)
        ok = 0;
    fill(c, 5, 5, 70);
    c.assign(a);                                            // shrink in place: 3 assigned, 2 destroyed
    if (c.size() != 3 || c.capacity() != 5 || c.m_start[0].ParticleName.v != 10 || c.m_start[2].PeriodicFrames != 17)
        ok = 0;
    fill(d, 6, 1, 90);
    d.assign(a);                                            // partial: 1 assigned, 2 copy-constructed at the end
    if (d.size() != 3 || d.capacity() != 6 || d.m_start[0].HelperName.v != 110 || d.m_start[2].ParticleName.v != 12)
        ok = 0;
    d.assign(d);                                            // self-assignment is a no-op
    if (d.size() != 3 || d.m_start[1].ParticleName.v != 11) ok = 0;
    // live strings: a 3, b 3, c 3, d 3 elements, two strings each
    if (g_Live != 24) ok = 0;
    a.clear(); b.clear(); c.clear(); d.clear();
    if (g_Live != 0) ok = 0;
    if (sizeof(ParticleAndEndPoints) != 20) ok = 0;
    if (!ok) { std::printf("0076b1e0_TEST FAIL\n"); return 1; }
    std::printf("0076b1e0_TEST PASS\n");
    return 0;
}