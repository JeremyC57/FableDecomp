// CMemoryAllocatorVariableSize::GetNoAreas() -- publish the two usage counters (+0xc18 / +0xc14) to the allocator statistics globals (0x013D28D8 / 0x013D28D4), then sum the area counts (+4) of the chunks on the list at +0xc1c (next at +0x20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64530, length 48 bytes.
// Leaf: GetNoAreas
// The chunk label (GetNoFreeAreas) is propagated. Shape: no frame, the sum in eax, `ret`.
struct SVarChunk_00a64530 {                           // a chunk of the variable-size allocator: areas +4, largest free +0x10, next at +0x20, first free area at +0x30
    long               m_0;
    long               m_noAreas;                  // +0x04
    long               m_8;
    long               m_c;
    unsigned long      m_largestFreeArea;          // +0x10
    unsigned char      _pad14[0xc];
    SVarChunk_00a64530*   m_pNext;                    // +0x20
    unsigned char      _pad24[0xc];
    void*              m_pFirstFreeArea;           // +0x30
};
extern long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

struct CMemoryAllocatorVariableSize {
    unsigned char     _pad00[0xc14];
    long              m_usage;                     // +0xc14
    long              m_chunkCounter;              // +0xc18
    SVarChunk_00a64530*  m_pFirstChunk;               // +0xc1c
    long GetNoAreas();
};

long CMemoryAllocatorVariableSize::GetNoAreas()
{
    g_LastAllocatorChunkCounter_013d28d8 = m_chunkCounter;
    g_LastAllocatorUsage_013d28d4 = m_usage;
    long total = 0;
    for (SVarChunk_00a64530* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext)
        total += chunk->m_noAreas;
    return total;
}