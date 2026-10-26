// Behaviour test for 0x0077EF30 std::vector<pair<CCharString, C3DVector> >::_M_insert_overflow (STLport growth path).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0;                          // live CCharString payloads

class CCharString { public:
    char* PStringData;
    CCharString(const char* s) { PStringData = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(PStringData, s); ++g_live; }
    CCharString(const CCharString& o) { PStringData = (char*)std::malloc(std::strlen(o.PStringData) + 1); std::strcpy(PStringData, o.PStringData); ++g_live; }
    ~CCharString() { std::free(PStringData); --g_live; }
};

class C3DVector { public: float X, Y, Z; };
struct CSkeletalMorph {                         // pair<CCharString, C3DVector>
    CCharString first;
    C3DVector   second;
    CSkeletalMorph(long id, const char* n, float s) : first(n) { second.X = (float)id; second.Y = s; second.Z = 0; }
};

struct __false_type { __false_type() { } };

struct Vec {
    CSkeletalMorph* m_start; CSkeletalMorph* m_finish; CSkeletalMorph* m_end;
    unsigned int size() const { return (unsigned int)(m_finish - m_start); }
    void insert_overflow(CSkeletalMorph* pos, const CSkeletalMorph& x, const __false_type&, unsigned int fill_len, bool atend)
    {
        const unsigned int old_size = size();
        const unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CSkeletalMorph* ns = len != 0 ? (CSkeletalMorph*)std::malloc(len * sizeof(CSkeletalMorph)) : 0;
        CSkeletalMorph* nf = ns;
        for (CSkeletalMorph* p = m_start; p != pos; ++p, ++nf) new (nf) CSkeletalMorph(*p);
        if (fill_len == 1) { new (nf) CSkeletalMorph(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CSkeletalMorph(x);
        if (!atend) for (CSkeletalMorph* p = pos; p != m_finish; ++p, ++nf) new (nf) CSkeletalMorph(*p);
        for (CSkeletalMorph* d = m_start; d != m_finish; ++d) d->~CSkeletalMorph();
        if (m_start) std::free(m_start);
        m_start = ns; m_finish = nf; m_end = ns + len;
    }
};

int main()
{
    int ok = 1;
    {
        Vec v; v.m_start = v.m_finish = v.m_end = 0;
        CSkeletalMorph a(1, "brow", 0.5f), b(2, "jaw", 0.25f), c(3, "nose", 1.0f);
        v.insert_overflow(v.m_finish, a, __false_type(), 1, true);
        v.insert_overflow(v.m_finish, b, __false_type(), 1, true);
        v.insert_overflow(v.m_start, c, __false_type(), 3, false);      // len = 2 + 3 = 5
        if (v.size() != 5 || v.m_end - v.m_start != 5) ok = 0;
        if (v.m_start[0].second.X != 3.0f || std::strcmp(v.m_start[2].first.PStringData, "nose") != 0) ok = 0;
        if (v.m_start[3].second.X != 1.0f || v.m_start[4].second.Y != 0.25f || std::strcmp(v.m_start[4].first.PStringData, "jaw") != 0) ok = 0;
        if (g_live != 3 + 5) ok = 0;                                    // old copies destroyed
        for (CSkeletalMorph* d = v.m_start; d != v.m_finish; ++d) d->~CSkeletalMorph();
        std::free(v.m_start);
    }
    if (g_live != 0) ok = 0;
    if (!ok) { std::printf("0077ef30_TEST FAIL\n"); return 1; }
    std::printf("0077ef30_TEST PASS\n");
    return 0;
}