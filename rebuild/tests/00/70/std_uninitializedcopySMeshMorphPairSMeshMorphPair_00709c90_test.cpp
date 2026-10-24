// Behaviour test for 0x00709C90 std::__uninitialized_copy<pair<CMeshInfo, CEngineTextureMorphSet>*>.
#include <cstdio>
#include <new>

static int g_arrayCopies = 0;
struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh : 1; };
struct CMorphArray {
    int* _M_start; int* _M_finish; int* _M_end_of_storage;
    CMorphArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    CMorphArray(const CMorphArray& o) : _M_start(o._M_start), _M_finish(o._M_finish), _M_end_of_storage(o._M_end_of_storage) { ++g_arrayCopies; }
};
struct CEngineTextureMorphSet { CMorphArray Morphs; };
struct SPair {
    CMeshInfo first; CEngineTextureMorphSet second;
    SPair() {}
    SPair(const SPair& o) : first(o.first), second(o.second) {}
};

static SPair* UninitializedCopy(SPair* first, SPair* last, SPair* result)
{
    SPair* cur = result;
    for ( ; first != last; ++first, ++cur)
        new (cur) SPair(*first);
    return cur;
}

int main()
{
    int morphs[4];
    SPair src[2];
    src[0].first.Index = 5; src[0].first.SizeX = 1.25f; src[0].first.PrimaryMesh = 1; src[0].second.Morphs._M_start = morphs; src[0].second.Morphs._M_finish = morphs + 2;
    src[1].first.Index = 9; src[1].first.SizeX = 2.5f; src[1].first.PrimaryMesh = 0;
    static unsigned char raw[sizeof(SPair) * 2];
    SPair* dst = (SPair*)raw;
    SPair* end = UninitializedCopy(src, src + 2, dst);
    bool ok = end == dst + 2 && g_arrayCopies == 2 && dst[0].first.Index == 5 && dst[0].first.SizeX == 1.25f && dst[0].first.PrimaryMesh == 1
           && dst[0].second.Morphs._M_start == morphs && dst[0].second.Morphs._M_finish == morphs + 2
           && dst[1].first.Index == 9 && dst[1].first.PrimaryMesh == 0 && dst[1].second.Morphs._M_start == 0;
    ok = ok && UninitializedCopy(src, src, dst) == dst && g_arrayCopies == 2;
    if (!ok) { std::printf("00709c90_TEST FAIL\n"); return 1; }
    std::printf("00709c90_TEST PASS\n");
    return 0;
}