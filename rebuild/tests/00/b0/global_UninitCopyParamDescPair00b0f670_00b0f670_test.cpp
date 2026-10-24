// Behaviour test for 0x00B0F670 UninitCopy_ParamDescPair_00b0f670: placement copy of pair<unsigned long, CParticleUpdateParamDesc>.
#include <cstdio>
#include <new>

struct __false_type { __false_type() { } };
struct CParticleUpdateParamCallback { int m_id; };
struct CParticleUpdateParamDesc {
    CParticleUpdateParamCallback* Callback;
    unsigned long                 Index;
    CParticleUpdateParamDesc() : Callback(0), Index(0) { }
    CParticleUpdateParamDesc(const CParticleUpdateParamDesc& o) : Callback(o.Callback), Index(o.Index) { }
    virtual int Kind() const { return 42; }
};
struct CParamDescPair {
    unsigned long            first;
    CParticleUpdateParamDesc second;
    CParamDescPair() : first(0) { }
    CParamDescPair(const CParamDescPair& o) : first(o.first), second(o.second) { }
};

static CParamDescPair* UninitCopy(const CParamDescPair* first, const CParamDescPair* last, CParamDescPair* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) CParamDescPair(*first);
    return result;
}

int main()
{
    CParticleUpdateParamCallback cb[3];
    CParamDescPair src[3];
    for (int i = 0; i < 3; ++i) { cb[i].m_id = i; src[i].first = 0x10u + i; src[i].second.Callback = &cb[i]; src[i].second.Index = 7u * i; }
    static double storage[3 * sizeof(CParamDescPair) / sizeof(double) + 1];
    CParamDescPair* dst = (CParamDescPair*)storage;
    __false_type tag;
    CParamDescPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    for (int k = 0; k < 3; ++k)
        if (dst[k].first != 0x10u + k || dst[k].second.Callback != &cb[k] || dst[k].second.Index != 7u * k || dst[k].second.Kind() != 42) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00b0f670_TEST FAIL\n"); return 1; }
    std::printf("00b0f670_TEST PASS\n");
    return 0;
}