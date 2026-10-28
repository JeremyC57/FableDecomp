#include <cstdio>
// Behaviour test for CPreallocTexturePool::CPreallocTexturePool (0x00A6A330).
static int g_list = 0;
class CResourceList { public: CResourceList() { ++g_list; } virtual ~CResourceList() { } };
struct Vec { void* s; void* f; void* e; Vec() : s(0), f(0), e(0) { } };
struct CPreallocTexturePool : CResourceList {
    unsigned long Count, MaxSize; long PoolIndex; Vec Textures;
    CPreallocTexturePool() : Count(0), MaxSize(0), PoolIndex(-1) { }
};
int main()
{
    CPreallocTexturePool p;
    bool ok = g_list == 1 && p.Count == 0 && p.MaxSize == 0 && p.PoolIndex == -1 && p.Textures.s == 0 && p.Textures.f == 0
           && p.Textures.e == 0;
    std::printf(ok ? "00a6a330_TEST PASS\n" : "00a6a330_TEST FAIL\n");
    return ok ? 0 : 1;
}