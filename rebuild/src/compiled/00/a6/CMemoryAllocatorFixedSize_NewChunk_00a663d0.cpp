// CMemoryAllocatorFixedSize::NewChunk() -- allocate ChunkSize bytes from the OS, build a chunk in them and link it at the head of the chunk list as one more empty chunk; on failure poke the global memory manager's out-of-memory hook and return 0
// Leaf: NewChunk
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A663D0, 122 bytes, plain `ret` (PDB ?NewChunk@CMemoryAllocatorFixedSize@@IAEPAVCMemoryAreaChunk@1@XZ).
// Callees: CMemoryAllocatorFixedSize::InternalAlloc(unsigned long, void**) @ 0x00A65F40, CreateChunk(void*) @ 0x00A661D0,
//   the global memory manager (pointer @ 0x013CAA90) hook @ 0x009F1F00 (an empty `ret` in the retail build).
//   Caller: CMemoryAllocatorFixedSize::Allocate @ 0x00A66450 (landed, which names this NewChunk too).
// Layout: engine/CMemoryAllocatorFixedSize.h (PFirstChunk +0xc1c, PLastChunk +0xc20, ChunkSize +0xc38, NoEmptyChunks
//   +0xc48); the fixed-size CMemoryAreaChunk is the PDB's 24-byte record (PNextChunk +8, PPreviousChunk +0xc).
// Shape: `void* mem; if (InternalAlloc(ChunkSize, &mem)) { chunk = CreateChunk(mem); InsertChunkAtListHead(chunk);
//   ++NoEmptyChunks; return chunk; }` with the list insert inline (PFirstChunk re-read after each store); the out-of-
//   memory hook path returns 0 at the end.
#include "engine/CMemoryAllocatorFixedSize.h"

struct CMemoryAreaChunk {                               // CMemoryAllocatorFixedSize::CMemoryAreaChunk (24 bytes)
    void*             ThisChunkMem;                     // +0x00
    long              NoAllocatedAreas;                 // +0x04
    CMemoryAreaChunk* PNextChunk;                       // +0x08
    CMemoryAreaChunk* PPreviousChunk;                   // +0x0c
    void*             PFirstFreeArea;                   // +0x10
    void*             PFirstAllocatedArea;              // +0x14
};

struct CGlobalMemoryManager {                           // descriptive: the object behind the 0x013CAA90 pointer
    void OnOutOfMemory();                               // 0x009F1F00
};
extern CGlobalMemoryManager* g_pGlobalMemoryManager;    // 0x013CAA90

struct CMemoryAllocatorFixedSizeChunks : CMemoryAllocatorFixedSize {
    unsigned long InternalAlloc(unsigned long size, void** mem);    // 0x00A65F40
    CMemoryAreaChunk* CreateChunk(void* mem);                        // 0x00A661D0
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
    CMemoryAreaChunk* NewChunk();
};

CMemoryAreaChunk* CMemoryAllocatorFixedSizeChunks::NewChunk()
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