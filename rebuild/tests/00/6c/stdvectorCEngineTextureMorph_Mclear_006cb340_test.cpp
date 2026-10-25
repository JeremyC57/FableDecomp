// Test for 0x006CB340 vector<CEngineTextureMorph>::_M_clear: the source object must free every record's non-null
// Blends buffer in order, then the record block; a null block start frees nothing. `free` is recorded here (the blocks
// are static storage, so nothing reaches the CRT heap).
#include <stdio.h>
#include <string.h>
static void* g_freed[16];
static int g_nfreed;
extern "C" void __cdecl free(void* p) { if (g_nfreed < 16) g_freed[g_nfreed] = p; ++g_nfreed; }

class CRGBColour { public: unsigned char B, G, R, A; };
class CCompositeBlendDesc {
public:
    unsigned long TextureIndex; long BlendType; CRGBColour DiffuseColour; CRGBColour SpecularColour;
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp> class vector {
public:
    _Tp* _M_start; _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    void _M_clear();
};
}
class CEngineTextureMorph {
public:
    long OriginalTextureIndex; long MorphID; std::vector<CCompositeBlendDesc> Blends;
};
static CEngineTextureMorph g_recs[4];
static CCompositeBlendDesc g_b0[2], g_b2[1];
int main()
{
    if (sizeof(CEngineTextureMorph) != 20) { puts("006cb340_TEST FAIL size"); return 1; }
    memset(g_recs, 0, sizeof(g_recs));
    g_recs[0].Blends._M_start = g_b0; g_recs[0].Blends._M_finish = g_b0 + 2; g_recs[0].Blends._M_end_of_storage._M_data = g_b0 + 2;
    g_recs[2].Blends._M_start = g_b2; g_recs[2].Blends._M_finish = g_b2 + 1; g_recs[2].Blends._M_end_of_storage._M_data = g_b2 + 1;
    std::vector<CEngineTextureMorph> v;
    v._M_start = g_recs; v._M_finish = g_recs + 3; v._M_end_of_storage._M_data = g_recs + 4;
    g_nfreed = 0;
    v._M_clear();
    if (g_nfreed != 3 || g_freed[0] != g_b0 || g_freed[1] != g_b2 || g_freed[2] != g_recs) { puts("006cb340_TEST FAIL clear"); return 2; }
    std::vector<CEngineTextureMorph> e; e._M_start = e._M_finish = e._M_end_of_storage._M_data = 0;
    g_nfreed = 0;
    e._M_clear();
    if (g_nfreed != 0) { puts("006cb340_TEST FAIL null"); return 3; }
    puts("006cb340_TEST PASS");
    return 0;
}
