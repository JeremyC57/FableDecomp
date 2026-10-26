// Behaviour test for 0x009C5310 CFileInstaller::InitBuffers.
#include <cstdio>
#include <cstdlib>
static int g_creates = 0, g_lockDepth = 0, g_allocs = 0, g_globalAllocs = 0, g_frees = 0;
class CMemoryAllocator { public: void* InternalAllocate(int size, unsigned int, bool) { ++g_allocs; return std::malloc(size); } };
static CMemoryAllocator g_alloc; static CMemoryAllocator* g_pGlobalAllocator = 0;
static void GFCreateGlobalAllocator() { ++g_creates; g_pGlobalAllocator = &g_alloc; }
static void* GFAllocateFromGlobalAllocator(int size)
{
    if (!g_pGlobalAllocator) GFCreateGlobalAllocator();
    ++g_lockDepth;
    void* block = g_pGlobalAllocator->InternalAllocate(size, 0, false);
    --g_lockDepth;
    return block;
}
static void* GlobalAlloc_(unsigned int, unsigned int bytes) { ++g_globalAllocs; return std::malloc(bytes); }
struct CBufferChunk { unsigned char* Mem; bool Used; bool IsInput; };
struct CBufferChunkArray {
    CBufferChunk* m_pFirst; unsigned int m_n;
    void Resize(unsigned int n) { m_pFirst = new CBufferChunk[n]; m_n = n; for (unsigned int i = 0; i < n; ++i) { m_pFirst[i].Mem = 0; m_pFirst[i].Used = false; } }
};
class CFileInstaller { public:
    CBufferChunkArray BufferChunks; unsigned char* PReadBuffer; unsigned char* PWriteBuffer; int AvailableBuffers;
    unsigned int BufferChunkSize, NoBufferChunks;
    void FreeBuffers() { ++g_frees; }
    void InitBuffers(unsigned int chunkSize, unsigned int chunkCount)
    {
        BufferChunkSize = chunkSize;
        NoBufferChunks = chunkCount;
        FreeBuffers();
        BufferChunks.Resize(chunkCount);
        for (unsigned int i = 0; i < chunkCount; ++i)
            BufferChunks.m_pFirst[i].Mem = (unsigned char*)GFAllocateFromGlobalAllocator(chunkSize);
        AvailableBuffers = chunkCount;
        PReadBuffer = (unsigned char*)GlobalAlloc_(0, chunkSize);
        PWriteBuffer = (unsigned char*)GlobalAlloc_(0, chunkSize);
    }
};
static int Fail() { std::printf("009c5310_TEST FAIL\n"); return 1; }
int main()
{
    CFileInstaller f;
    f.InitBuffers(64, 3);
    if (f.BufferChunkSize != 64 || f.NoBufferChunks != 3 || g_frees != 1 || g_creates != 1 || g_allocs != 3 || g_lockDepth != 0) return Fail();
    for (int i = 0; i < 3; ++i) if (!f.BufferChunks.m_pFirst[i].Mem) return Fail();
    if (f.AvailableBuffers != 3 || g_globalAllocs != 2 || !f.PReadBuffer || !f.PWriteBuffer) return Fail();
    f.InitBuffers(32, 0);
    if (g_creates != 1 || g_allocs != 3 || f.AvailableBuffers != 0 || g_globalAllocs != 4) return Fail();
    std::printf("009c5310_TEST PASS\n");
    return 0;
}