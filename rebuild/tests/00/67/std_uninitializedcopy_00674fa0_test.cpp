#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_vectorCopies = 0;
struct CTextureMorphVector {
    void* _M_start; void* _M_finish; void* _M_end_of_storage;
    CTextureMorphVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CTextureMorphVector(const CTextureMorphVector& other) : _M_start(other._M_start), _M_finish(other._M_finish), _M_end_of_storage(other._M_end_of_storage) { ++g_vectorCopies; }
};
struct CKeyPairCompareLess { };
struct CTextureMorphMap : CTextureMorphVector { CKeyPairCompareLess Compare; bool Dirty; CTextureMorphMap() : Dirty(false) { } };
struct CBodyPartMesh { long MeshIndex; long Priority; CTextureMorphMap TextureMorphs; CBodyPartMesh() : MeshIndex(0), Priority(0) { } };
static char g_blocks[4][8];
typedef CBodyPartMesh TElem;
static void Fill(TElem& e, int i)
{
    e.MeshIndex = 30 + i; e.Priority = -i; e.TextureMorphs._M_start = g_blocks[i]; e.TextureMorphs._M_finish = g_blocks[i] + 4; e.TextureMorphs._M_end_of_storage = g_blocks[i] + 8; e.TextureMorphs.Dirty = i == 2;
}
static bool Check(const TElem& e, int i)
{
    return e.MeshIndex == 30 + i && e.Priority == -i && e.TextureMorphs._M_start == g_blocks[i] && e.TextureMorphs._M_finish == g_blocks[i] + 4 && e.TextureMorphs.Dirty == (i == 2);
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    if (g_vectorCopies != 3) ok = false;
    if (!ok) { std::printf("00674fa0_TEST FAIL\n"); return 1; }
    std::printf("00674fa0_TEST PASS\n");
    return 0;
}