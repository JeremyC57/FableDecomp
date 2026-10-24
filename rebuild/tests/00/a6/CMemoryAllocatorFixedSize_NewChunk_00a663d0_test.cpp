// Behaviour test for 0x00A663D0 CMemoryAllocatorFixedSize::NewChunk()
#include <cstdio>

struct CMemoryAreaChunk {
    void*             ThisChunkMem;
    long              NoAllocatedAreas;
    CMemoryAreaChunk* PNextChunk;
    CMemoryAreaChunk* PPreviousChunk;
    void*             PFirstFreeArea;
    void*             PFirstAllocatedArea;
};

static int g_outOfMemoryCalls = 0;
struct CGlobalMemoryManager {
    void OnOutOfMemory() { ++g_outOfMemoryCalls; }      // 0x009F1F00 is an empty hook in retail
};
static CGlobalMemoryManager g_manager;
static CGlobalMemoryManager* g_pGlobalMemoryManager = &g_manager;

static bool g_allocSucceeds = true;
static unsigned long g_lastAllocSize = 0;
static CMemoryAreaChunk g_chunkStore[4];
static int g_nextChunk = 0;

struct CMemoryAllocatorFixedSize {                      // the members NewChunk touches (engine/CMemoryAllocatorFixedSize.h)
    CMemoryAreaChunk* PFirstChunk;
    CMemoryAreaChunk* PLastChunk;
    unsigned long     ChunkSize;
    long              NoEmptyChunks;
};

struct CMemoryAllocatorFixedSizeChunks : CMemoryAllocatorFixedSize {
    unsigned long InternalAlloc(unsigned long size, void** mem)          // 0x00A65F40 stand-in
    {
        g_lastAllocSize = size;
        if (!g_allocSucceeds) return 0;
        *mem = &g_chunkStore[g_nextChunk++];
        return 1;
    }
    CMemoryAreaChunk* CreateChunk(void* mem)                              // 0x00A661D0 stand-in
    {
        CMemoryAreaChunk* chunk = (CMemoryAreaChunk*)mem;
        chunk->ThisChunkMem = mem;
        chunk->NoAllocatedAreas = 0;
        chunk->PNextChunk = chunk->PPreviousChunk = 0;
        chunk->PFirstFreeArea = chunk->PFirstAllocatedArea = 0;
        return chunk;
    }
    void InsertChunkAtListHead(CMemoryAreaChunk* chunk)
    {
        chunk->PNextChunk = PFirstChunk;
        if (PFirstChunk)
            PFirstChunk->PPreviousChunk = chunk;
        chunk->PPreviousChunk = 0;
        if (!PFirstChunk)
            PLastChunk = chunk;
        PFirstChunk = chunk;
    }
    CMemoryAreaChunk* NewChunk()
    {
        void* mem;
        if (InternalAlloc(ChunkSize, &mem)) {
            CMemoryAreaChunk* chunk = CreateChunk(mem);
            InsertChunkAtListHead(chunk);
            ++NoEmptyChunks;
            return chunk;
        }
        g_pGlobalMemoryManager->OnOutOfMemory();
        return 0;
    }
};

int main()
{
    CMemoryAllocatorFixedSizeChunks a;
    a.PFirstChunk = a.PLastChunk = 0;
    a.ChunkSize = 0x10000;
    a.NoEmptyChunks = 0;
    CMemoryAreaChunk* c1 = a.NewChunk();
    CMemoryAreaChunk* c2 = a.NewChunk();
    bool ok = c1 == &g_chunkStore[0] && c2 == &g_chunkStore[1] && g_lastAllocSize == 0x10000 &&
              a.PFirstChunk == c2 && a.PLastChunk == c1 && c2->PNextChunk == c1 && c1->PPreviousChunk == c2 &&
              c2->PPreviousChunk == 0 && c1->PNextChunk == 0 && a.NoEmptyChunks == 2 && g_outOfMemoryCalls == 0;
    g_allocSucceeds = false;
    CMemoryAreaChunk* c3 = a.NewChunk();
    ok = ok && c3 == 0 && g_outOfMemoryCalls == 1 && a.NoEmptyChunks == 2 && a.PFirstChunk == c2;
    if (!ok) { std::printf("00a663d0_TEST FAIL\n"); return 1; }
    std::printf("00a663d0_TEST PASS\n");
    return 0;
}