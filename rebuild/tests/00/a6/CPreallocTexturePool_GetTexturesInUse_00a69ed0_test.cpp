// Behaviour test for CPreallocTexturePool::GetTexturesInUse (0x00A69ED0): count allocated textures whose unused-frame
// count is within the limit; unallocated textures are not queried.
#include <cstdio>
static int g_queries = 0;
struct CManagedTextureData { unsigned long unused; bool Allocated; unsigned long GetUnusedFrames() const { ++g_queries; return unused; } };
struct CPtr { CManagedTextureData* Data; CManagedTextureData* operator->() const { return Data; } };
struct CArr { CPtr* m_pFirst; CPtr* m_pLast; CPtr* m_pEnd; const CPtr& operator[](unsigned long i) const { return m_pFirst[i]; } };
struct CPreallocTexturePool {
    unsigned long Count; CArr Textures;
    unsigned long GetTexturesInUse(unsigned long maxUnusedFrames) const
    {
        unsigned long inUse = 0;
        for (unsigned long i = 0; i < Count; ++i) {
            if (Textures[i]->Allocated && Textures[i]->GetUnusedFrames() <= maxUnusedFrames)
                ++inUse;
        }
        return inUse;
    }
};
int main()
{
    CManagedTextureData t[4];
    t[0].Allocated = true; t[0].unused = 0; t[1].Allocated = true; t[1].unused = 5; t[2].Allocated = false; t[2].unused = 0; t[3].Allocated = true; t[3].unused = 2;
    CPtr p[4]; for (int i = 0; i < 4; ++i) p[i].Data = &t[i];
    CPreallocTexturePool pool; pool.Count = 4; pool.Textures.m_pFirst = p; pool.Textures.m_pLast = p + 4; pool.Textures.m_pEnd = p + 4;
    bool ok = pool.GetTexturesInUse(2) == 2 && g_queries == 3 && pool.GetTexturesInUse(5) == 3 && pool.GetTexturesInUse(0) == 1;
    pool.Count = 0; ok = ok && pool.GetTexturesInUse(9) == 0;
    if (!ok) { std::printf("00a69ed0_TEST FAIL\n"); return 1; }
    std::printf("00a69ed0_TEST PASS\n");
    return 0;
}