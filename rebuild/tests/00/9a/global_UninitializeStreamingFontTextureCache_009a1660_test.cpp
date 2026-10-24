// Behaviour test for 0x009A1660 UninitializeStreamingFontTextureCache: with a cache block present, the four arrays
// are delete[]d and nulled, the block is returned to the global allocator (which is destroyed when flagged and
// empty) and nulled; without a block nothing happens.
#include <cstdio>

static int g_arraysFreed = 0;
static int g_blocksFreed = 0;
static int g_allocatorDestroyed = 0;
static int g_lockDepth = 0;

struct CMemoryAllocator {
    unsigned int NoAllocations;
    void InternalDeallocate(void*) { ++g_blocksFreed; --NoAllocations; }
    unsigned int GetNoAllocations() { return NoAllocations; }
    ~CMemoryAllocator() { ++g_allocatorDestroyed; }
    static void EnterSynchronisation() { ++g_lockDepth; }
    static void LeaveSynchronisation() { --g_lockDepth; }
};

static CMemoryAllocator* g_pGlobalAllocator = 0;
static bool g_bDestroyAllocatorWhenEmpty = false;

static void* g_pStreamingFontCacheMemory = 0;
static unsigned int*   g_pStreamingFontCacheDwordTable = 0;
static unsigned char*  g_pStreamingFontCacheByteTable = 0;
static unsigned int*   g_pStreamingFontCachePairTable = 0;
static unsigned short* g_pStreamingFontCacheWordTable = 0;

template <class T> static void DeleteArray(T* p) { if (p) ++g_arraysFreed; delete[] p; }

static void GFFreeFromGlobalAllocator(void* block)
{
    CMemoryAllocator::EnterSynchronisation();
    if (block && g_pGlobalAllocator) {
        g_pGlobalAllocator->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty) {
            if (g_pGlobalAllocator->GetNoAllocations() == 0) {
                delete g_pGlobalAllocator;
                g_pGlobalAllocator = 0;
            }
        }
    }
    CMemoryAllocator::LeaveSynchronisation();
}

static void UninitializeStreamingFontTextureCache()
{
    if (g_pStreamingFontCacheMemory != 0) {
        DeleteArray(g_pStreamingFontCacheWordTable);
        g_pStreamingFontCacheWordTable = 0;
        DeleteArray(g_pStreamingFontCacheDwordTable);
        g_pStreamingFontCacheDwordTable = 0;
        DeleteArray(g_pStreamingFontCacheByteTable);
        g_pStreamingFontCacheByteTable = 0;
        DeleteArray(g_pStreamingFontCachePairTable);
        g_pStreamingFontCachePairTable = 0;
        GFFreeFromGlobalAllocator(g_pStreamingFontCacheMemory);
        g_pStreamingFontCacheMemory = 0;
    }
}

int main()
{
    static char block[16];
    int bad = 0;
    // nothing cached: no-op
    g_pStreamingFontCacheWordTable = new unsigned short[4];
    UninitializeStreamingFontTextureCache();
    if (g_arraysFreed != 0 || g_pStreamingFontCacheWordTable == 0) bad = 1;
    // cached, allocator flagged to die when empty, last allocation
    g_pStreamingFontCacheMemory = block;
    g_pStreamingFontCacheDwordTable = new unsigned int[5];
    g_pStreamingFontCacheByteTable = new unsigned char[6];
    g_pStreamingFontCachePairTable = new unsigned int[8];
    g_pGlobalAllocator = new CMemoryAllocator;
    g_pGlobalAllocator->NoAllocations = 1;
    g_bDestroyAllocatorWhenEmpty = true;
    UninitializeStreamingFontTextureCache();
    if (g_arraysFreed != 4 || g_blocksFreed != 1 || g_allocatorDestroyed != 1 || g_pGlobalAllocator != 0) bad = 1;
    if (g_pStreamingFontCacheMemory || g_pStreamingFontCacheWordTable || g_pStreamingFontCacheDwordTable ||
        g_pStreamingFontCacheByteTable || g_pStreamingFontCachePairTable || g_lockDepth != 0) bad = 1;
    // cached, allocator kept (still has allocations)
    g_pStreamingFontCacheMemory = block;
    CMemoryAllocator keep; keep.NoAllocations = 3;
    g_pGlobalAllocator = &keep;
    UninitializeStreamingFontTextureCache();
    if (g_blocksFreed != 2 || g_pGlobalAllocator != &keep || keep.NoAllocations != 2 || g_pStreamingFontCacheMemory != 0) bad = 1;
    g_pGlobalAllocator = 0;
    if (bad) { std::printf("009a1660_TEST FAIL\n"); return 1; }
    std::printf("009a1660_TEST PASS\n");
    return 0;
}