// CMemoryAllocatorFixedSize::DeleteChunk(CMemoryAreaChunk* chunk) -- retire one chunk: clear the current-allocation chunk if it is this one, unlink it from the doubly linked chunk list (head/tail advanced), and return its raw block to the process heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66160, length 105 bytes.
// Callees: KERNEL32 GetProcessHeap (IAT 0x0143FD3C), HeapFree (IAT 0x0143FE8C) -- HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, chunk->ThisChunkMem).
// Layout (PDB): CMemoryAreaChunk (fixed-size flavour, 0x18) ThisChunkMem +0, NoAllocatedAreas +4, PNextChunk +8, PPreviousChunk +0xc,
//   PFirstFreeArea +0x10, PFirstAllocatedArea +0x14; CMemoryAllocatorFixedSize PFirstChunk +0xc1c, PLastChunk +0xc20, PCurrentAllocatorChunk +0xc24.
//   (engine/CMemoryAllocatorFixedSize.h is all padding for this range, so the PDB members are modelled locally.)
// Shape: member-first compares (`cmp [ecx+0C24h],eax`), the imports through their IAT slots (`__declspec(dllimport)`), `ret 4`.
extern "C" __declspec(dllimport) void* __stdcall GetProcessHeap();                                    // IAT 0x0143FD3C
extern "C" __declspec(dllimport) int __stdcall HeapFree(void* heap, unsigned long flags, void* mem);   // IAT 0x0143FE8C

struct CMemoryArea;
struct CMemoryAreaChunk {
    void*             ThisChunkMem;                // +0x00 the raw heap block the chunk was carved from
    long              NoAllocatedAreas;            // +0x04
    CMemoryAreaChunk* PNextChunk;                  // +0x08
    CMemoryAreaChunk* PPreviousChunk;              // +0x0c
    CMemoryArea*      PFirstFreeArea;              // +0x10
    CMemoryArea*      PFirstAllocatedArea;         // +0x14
};
struct CMemoryAllocatorFixedSize {
    unsigned char     _base[0xc1c];                // CMemoryAllocatorBase (vptr, CPerformanceStats, Alignment, AlignmentShift)
    CMemoryAreaChunk* PFirstChunk;                 // +0xc1c
    CMemoryAreaChunk* PLastChunk;                  // +0xc20
    CMemoryAreaChunk* PCurrentAllocatorChunk;      // +0xc24
    void DeleteChunk(CMemoryAreaChunk* chunk);
};

void CMemoryAllocatorFixedSize::DeleteChunk(CMemoryAreaChunk* chunk)
{
    if (PCurrentAllocatorChunk == chunk)
        PCurrentAllocatorChunk = 0;
    if (chunk->PNextChunk)
        chunk->PNextChunk->PPreviousChunk = chunk->PPreviousChunk;
    if (chunk->PPreviousChunk)
        chunk->PPreviousChunk->PNextChunk = chunk->PNextChunk;
    if (PFirstChunk == chunk)
        PFirstChunk = chunk->PNextChunk;
    if (PLastChunk == chunk)
        PLastChunk = chunk->PPreviousChunk;
    HeapFree(GetProcessHeap(), 1, chunk->ThisChunkMem);
}