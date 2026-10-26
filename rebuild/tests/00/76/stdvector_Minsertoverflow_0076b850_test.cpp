// Behaviour test for 0x0076B850 std::vector<ParticleAndEndPoints>::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0;

class CCharString { public:
    char* PStringData;
    CCharString(const char* s) { PStringData = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(PStringData, s); ++g_live; }
    CCharString(const CCharString& o) { PStringData = (char*)std::malloc(std::strlen(o.PStringData) + 1); std::strcpy(PStringData, o.PStringData); ++g_live; }
    ~CCharString() { std::free(PStringData); --g_live; }
};

struct ParticleAndEndPoints {
    CCharString ParticleName;
    CCharString HelperName;
    long        StartPoint;
    long        EndPoint;
    long        PeriodicFrames;
    ParticleAndEndPoints(const char* p, const char* h, long s, long e, long f)
        : ParticleName(p), HelperName(h), StartPoint(s), EndPoint(e), PeriodicFrames(f) { }
};

struct __false_type { __false_type() { } };

struct Vec {
    ParticleAndEndPoints* m_start; ParticleAndEndPoints* m_finish; ParticleAndEndPoints* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void clear_()                                                   // 0x0076B1A0
    {
        for (ParticleAndEndPoints* d = m_start; d != m_finish; ++d) d->~ParticleAndEndPoints();
        if (m_start) std::free(m_start);
    }
    void insert_overflow(ParticleAndEndPoints* pos, const ParticleAndEndPoints& x, const __false_type&,
                         unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        ParticleAndEndPoints* ns = len != 0 ? (ParticleAndEndPoints*)std::malloc(len * sizeof(ParticleAndEndPoints)) : 0;
        ParticleAndEndPoints* nf = ns;
        for (ParticleAndEndPoints* p = m_start; p != pos; ++p, ++nf) new (nf) ParticleAndEndPoints(*p);
        if (fill_len == 1) { new (nf) ParticleAndEndPoints(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) ParticleAndEndPoints(x);
        if (!atend) for (ParticleAndEndPoints* p = pos; p != m_finish; ++p, ++nf) new (nf) ParticleAndEndPoints(*p);
        clear_();
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

int main()
{
    int ok = 1;
    {
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        ParticleAndEndPoints a("spark", "hand", 1, 2, 3), b("smoke", "head", 4, 5, 6);
        v.insert_overflow(v.m_finish, a, __false_type(), 1, true);
        v.insert_overflow(v.m_start, b, __false_type(), 2, false);      // len = 1 + 2 = 3
        if (v.size() != 3 || v.m_end - v.m_start != 3) ok = 0;
        if (std::strcmp(v.m_start[0].ParticleName.PStringData, "smoke") != 0 || v.m_start[1].PeriodicFrames != 6) ok = 0;
        if (std::strcmp(v.m_start[2].HelperName.PStringData, "hand") != 0 || v.m_start[2].EndPoint != 2) ok = 0;
        if (g_live != 4 + 6) ok = 0;
        v.clear_();
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("0076b850_TEST FAIL\n"); return 1; }
    std::printf("0076b850_TEST PASS\n");
    return 0;
}