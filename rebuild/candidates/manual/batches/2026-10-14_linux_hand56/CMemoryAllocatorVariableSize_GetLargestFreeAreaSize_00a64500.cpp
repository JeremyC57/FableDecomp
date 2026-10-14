// CMemoryAllocatorVariableSize::GetLargestFreeAreaSize() -- the largest of the chunks' largest-free-area sizes (+0x10 of each chunk on the list at +0xc1c, next at +0x20)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64500, length 48 bytes.
// Leaf: GetLargestFreeAreaSize
// Shape: no frame, unsigned max in eax, the loop head aligned, `ret`.
struct SVarChunk_00a64500 {                           // a chunk of the variable-size allocator: areas +4, largest free +0x10, next at +0x20, first free area at +0x30
    long               m_0;
    long               m_noAreas;                  // +0x04
    long               m_8;
    long               m_c;
    unsigned long      m_largestFreeArea;          // +0x10
    unsigned char      _pad14[0xc];
    SVarChunk_00a64500*   m_pNext;                    // +0x20
    unsigned char      _pad24[0xc];
    void*              m_pFirstFreeArea;           // +0x30
};
extern long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

struct CMemoryAllocatorVariableSize {
    unsigned char     _pad00[0xc14];
    long              m_usage;                     // +0xc14
    long              m_chunkCounter;              // +0xc18
    SVarChunk_00a64500*  m_pFirstChunk;               // +0xc1c
    unsigned long GetLargestFreeAreaSize();
};

unsigned long CMemoryAllocatorVariableSize::GetLargestFreeAreaSize()
{
    unsigned long largest = 0;
    for (SVarChunk_00a64500* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) {
        if (chunk->m_largestFreeArea > largest)
            largest = chunk->m_largestFreeArea;
    }
    return largest;
}
