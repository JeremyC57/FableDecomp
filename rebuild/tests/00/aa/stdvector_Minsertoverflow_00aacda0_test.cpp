// Behaviour test for 0x00AACDA0 std::vector<CActiveSoundInfo>::_M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_dtors = 0;
class CBaseClass { public: CBaseClass() { } virtual ~CBaseClass() { } };
class CActiveSoundInfo : public CBaseClass {
public:
    int m_sound, m_owner, m_info;
    CActiveSoundInfo(int s, int o, int i) : m_sound(s), m_owner(o), m_info(i) { }
    virtual ~CActiveSoundInfo() { ++g_dtors; }
};

struct Vec {
    CActiveSoundInfo* start; CActiveSoundInfo* finish; CActiveSoundInfo* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CActiveSoundInfo* pos, const CActiveSoundInfo& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CActiveSoundInfo* ns = len ? (CActiveSoundInfo*)std::malloc(len * sizeof(CActiveSoundInfo)) : 0;
        CActiveSoundInfo* nf = ns;
        for (CActiveSoundInfo* p = start; p != pos; ++p, ++nf) new (nf) CActiveSoundInfo(*p);
        if (fill_len == 1) { new (nf) CActiveSoundInfo(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CActiveSoundInfo(x);
        if (!atend) for (CActiveSoundInfo* p = pos; p != finish; ++p, ++nf) new (nf) CActiveSoundInfo(*p);
        for (CActiveSoundInfo* p = start; p != finish; ++p) p->~CActiveSoundInfo();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    CActiveSoundInfo a(1, 2, 3), b(4, 5, 6);
    v.insert_overflow(v.finish, a, 1, true);           // push_back into an empty vector
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].m_info != 3 || g_dtors != 0) { std::printf("00aacda0_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start, b, 2, false);           // two copies in front: len = 1 + 2
    if (v.size() != 3 || v.start[0].m_sound != 4 || v.start[1].m_owner != 5 || v.start[2].m_sound != 1 || g_dtors != 1) { std::printf("00aacda0_TEST FAIL\n"); return 1; }
    for (CActiveSoundInfo* p = v.start; p != v.finish; ++p) p->~CActiveSoundInfo();
    std::free(v.start);
    std::printf("00aacda0_TEST PASS\n");
    return 0;
}