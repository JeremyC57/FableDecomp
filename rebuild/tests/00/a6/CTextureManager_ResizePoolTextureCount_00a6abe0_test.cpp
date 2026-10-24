// Behaviour test for 0x00A6ABE0 CTextureManager::ResizePoolTextureCount(unsigned long, unsigned long)
#include <cstdio>

static int g_frameEnds = 0;

class CPreallocTexturePool {
public:
    unsigned long Count;
    bool ResizePool(unsigned long count) { Count = count; return count != 0; }   // 0x00A6A670 stand-in
};

class CTextureManager {
public:
    bool                 Initialised;
    unsigned long        PoolCount;
    unsigned long        PreallocSizes[16];
    CPreallocTexturePool PreallocPools[16];
    void FrameEnd() { ++g_frameEnds; }                                         // 0x00A698B0 stand-in
    long GetPoolIndex(unsigned long textureSize)                               // 0x00A69730 stand-in
    {
        for (unsigned long i = 0; i < PoolCount; ++i)
            if (PreallocSizes[i] == textureSize) return (long)i;
        return -1;
    }
    bool ResizePoolTextureCount(unsigned long textureSize, unsigned long count)
    {
        bool result = false;
        unsigned long index = GetPoolIndex(textureSize);
        if (index < PoolCount) {
            FrameEnd();
            result = PreallocPools[index].ResizePool(count);
        }
        return result;
    }
};

int main()
{
    CTextureManager m;
    m.PoolCount = 3;
    for (unsigned long i = 0; i < 16; ++i) {
        m.PreallocSizes[i] = 0x1000u << i;
        m.PreallocPools[i].Count = 0;
    }
    bool ok = m.ResizePoolTextureCount(0x2000, 12) && m.PreallocPools[1].Count == 12 && g_frameEnds == 1;
    ok = ok && !m.ResizePoolTextureCount(0x8000, 5) && g_frameEnds == 1;      // pool 3 is beyond PoolCount: -1
    ok = ok && !m.ResizePoolTextureCount(0x4000, 0) && g_frameEnds == 2;      // the pool's own false comes back
    if (!ok) { std::printf("00a6abe0_TEST FAIL\n"); return 1; }
    std::printf("00a6abe0_TEST PASS\n");
    return 0;
}