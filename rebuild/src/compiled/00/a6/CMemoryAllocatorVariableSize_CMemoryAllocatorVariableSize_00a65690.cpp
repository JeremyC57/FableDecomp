// CMemoryAllocatorVariableSize::~CMemoryAllocatorVariableSize() -- publish the two usage counters to the allocator statistics globals, then free every memory chunk still on the chain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65690, length 74 bytes.
// Callees: the chunk release @ 0x00A65390 (on `this`, one argument: the chunk; manifest "CMemoryAllocatorVariableSize_Deallocate"). Globals 0x013D28D8 /
// 0x013D28D4 (the last allocator's counters). Vtable 0x0129DB1C = CMemoryAllocatorVariableSize (rtti-vtables.tsv): a destructor storing its own vtable;
// the chunk label "CDrawGuildSeal::ClearMessages" is propagated. PDB CMemoryAllocatorVariableSize (3128 bytes): PFirstChunk +0xc1c; the two counters at
// +0xc14 / +0xc18 are unnamed in the PDB. Shape: `while (PFirstChunk) ReleaseChunk(PFirstChunk)` re-reads the member after every call (the release
// unlinks it); the loop head is aligned with a `lea ebx,[ebx]` nop.
struct CMemoryAreaChunk;

extern CMemoryAreaChunk* g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern long g_LastAllocatorUsage_013d28d4;                       // 0x013D28D4

struct CMemoryAllocatorVariableSize {              // vtable 0x0129DB1C
    virtual void v00();
    unsigned char     _pad04[0xc14 - 4];
    long              m_usage;                     // +0xc14
    CMemoryAreaChunk* m_chunkCounter;              // +0xc18
    CMemoryAreaChunk* PFirstChunk;                 // +0xc1c
    void ReleaseChunk(CMemoryAreaChunk* chunk);    // 0x00A65390
    ~CMemoryAllocatorVariableSize();
};

CMemoryAllocatorVariableSize::~CMemoryAllocatorVariableSize()
{
    g_LastAllocatorChunkCounter_013d28d8 = m_chunkCounter;
    g_LastAllocatorUsage_013d28d4 = m_usage;
    while (PFirstChunk)
        ReleaseChunk(PFirstChunk);
}