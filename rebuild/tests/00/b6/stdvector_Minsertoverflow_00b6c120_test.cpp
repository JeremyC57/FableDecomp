// Behaviour test for 0x00B6C120 std::vector<CVertexShader>::_M_insert_overflow
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_dtors = 0;
class CBaseClass { public: CBaseClass() { } virtual ~CBaseClass() { } };
class CVertexShaderStandIn : public CBaseClass {
public:
    int m_sound, m_owner, m_info;
    CVertexShaderStandIn(int s, int o, int i) : m_sound(s), m_owner(o), m_info(i) { }
    virtual ~CVertexShaderStandIn() { ++g_dtors; }
};

struct Vec {
    CVertexShaderStandIn* start; CVertexShaderStandIn* finish; CVertexShaderStandIn* eos;
    unsigned int size() const { return (unsigned int)(finish - start); }
    void insert_overflow(CVertexShaderStandIn* pos, const CVertexShaderStandIn& x, unsigned int fill_len, bool atend)
    {
        unsigned int old_size = size();
        unsigned int len = old_size + (old_size < fill_len ? fill_len : old_size);
        CVertexShaderStandIn* ns = len ? (CVertexShaderStandIn*)std::malloc(len * sizeof(CVertexShaderStandIn)) : 0;
        CVertexShaderStandIn* nf = ns;
        for (CVertexShaderStandIn* p = start; p != pos; ++p, ++nf) new (nf) CVertexShaderStandIn(*p);
        if (fill_len == 1) { new (nf) CVertexShaderStandIn(x); ++nf; }
        else for (unsigned int n = fill_len; n > 0; --n, ++nf) new (nf) CVertexShaderStandIn(x);
        if (!atend) for (CVertexShaderStandIn* p = pos; p != finish; ++p, ++nf) new (nf) CVertexShaderStandIn(*p);
        for (CVertexShaderStandIn* p = start; p != finish; ++p) p->~CVertexShaderStandIn();
        if (start) std::free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};

int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    CVertexShaderStandIn a(1, 2, 3), b(4, 5, 6);
    v.insert_overflow(v.finish, a, 1, true);           // push_back into an empty vector
    if (v.size() != 1 || v.eos - v.start != 1 || v.start[0].m_info != 3 || g_dtors != 0) { std::printf("00b6c120_TEST FAIL\n"); return 1; }
    v.insert_overflow(v.start, b, 2, false);           // two copies in front: len = 1 + 2
    if (v.size() != 3 || v.start[0].m_sound != 4 || v.start[1].m_owner != 5 || v.start[2].m_sound != 1 || g_dtors != 1) { std::printf("00b6c120_TEST FAIL\n"); return 1; }
    for (CVertexShaderStandIn* p = v.start; p != v.finish; ++p) p->~CVertexShaderStandIn();
    std::free(v.start);
    std::printf("00b6c120_TEST PASS\n");
    return 0;
}