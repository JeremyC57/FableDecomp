// Behaviour test for 0x0070AAF0 std::__uninitialized_fill_n<CEngineTextureDecal*> (deep copy of the blend list).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

struct CCompositeBlendDesc { unsigned int TextureIndex; int BlendType; unsigned int Diffuse; unsigned int Specular; };
struct CBlendVector {
    CCompositeBlendDesc* _M_start; CCompositeBlendDesc* _M_finish; CCompositeBlendDesc* _M_end_of_storage;
    CBlendVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CBlendVector(const CBlendVector& x) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        unsigned int n = (unsigned int)(x._M_finish - x._M_start);
        _M_start = n != 0 ? (CCompositeBlendDesc*)std::malloc(n * sizeof(CCompositeBlendDesc)) : 0;
        _M_finish = _M_start;
        _M_end_of_storage = _M_start + n;
        for (const CCompositeBlendDesc* p = x._M_start; p != x._M_finish; ++p, ++_M_finish) new (_M_finish) CCompositeBlendDesc(*p);
    }
};
struct CEngineTextureDecal { int OriginalTextureIndex; int DecalID; CBlendVector OverlaidBlends; };

static CEngineTextureDecal* UninitializedFillN(CEngineTextureDecal* first, unsigned int n, const CEngineTextureDecal& x)
{
    CEngineTextureDecal* cur = first;
    for ( ; n > 0; --n, ++cur)
        new (cur) CEngineTextureDecal(x);
    return cur;
}

int main()
{
    CCompositeBlendDesc blends[2] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } };
    CEngineTextureDecal x; x.OriginalTextureIndex = 12; x.DecalID = 34;
    x.OverlaidBlends._M_start = blends; x.OverlaidBlends._M_finish = blends + 2; x.OverlaidBlends._M_end_of_storage = blends + 2;
    static unsigned char raw[sizeof(CEngineTextureDecal) * 3];
    CEngineTextureDecal* dst = (CEngineTextureDecal*)raw;
    CEngineTextureDecal* end = UninitializedFillN(dst, 3, x);
    bool ok = end == dst + 3;
    for (int i = 0; i < 3; ++i)
        ok = ok && dst[i].OriginalTextureIndex == 12 && dst[i].DecalID == 34 && dst[i].OverlaidBlends._M_finish - dst[i].OverlaidBlends._M_start == 2
                && dst[i].OverlaidBlends._M_start != blends && dst[i].OverlaidBlends._M_start[1].Specular == 8
                && dst[i].OverlaidBlends._M_end_of_storage == dst[i].OverlaidBlends._M_finish;
    CEngineTextureDecal empty; empty.OriginalTextureIndex = 1; empty.DecalID = 2;
    end = UninitializedFillN(dst, 1, empty);
    ok = ok && end == dst + 1 && dst[0].OverlaidBlends._M_start == 0 && dst[0].OverlaidBlends._M_finish == 0;
    ok = ok && UninitializedFillN(dst, 0, x) == dst;
    if (!ok) { std::printf("0070aaf0_TEST FAIL\n"); return 1; }
    std::printf("0070aaf0_TEST PASS\n");
    return 0;
}