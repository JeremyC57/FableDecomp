// CMemoryAllocatorVariableSize::Validate() -- publish the two usage counters to the statistics globals, then for every area of every chunk (+0xc1c, next at +0x20; areas from +0x30, next at +0) ask the global allocator (0x013CAA90, 0x009F1F40) to check the payload (header + the aligned header size, both re-read from the globals each time)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65330, length 96 bytes.
// Leaf: Validate
// Callees: 0x009F1F40 (thiscall on the global allocator). Shape: the aligned header size recomputed from the two GLOBALS inside the loop (the call may change them), the chunk in edi, the area in esi, `ret`.
struct SArea_00a65330 {
    SArea_00a65330* m_pNext;                       // +0x00
};
struct SVarChunk_00a65330 {
    unsigned char       _pad00[0x20];
    SVarChunk_00a65330* m_pNext;                   // +0x20
    unsigned char       _pad24[0xc];
    SArea_00a65330*     m_pFirstArea;              // +0x30
};
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4
struct CMemoryAllocator_00a65330 {
    void ValidateBlock(void* block);               // 0x009F1F40
};
extern CMemoryAllocator_00a65330* g_pGlobalAllocator_013caa90;   // 0x013CAA90

struct CMemoryAllocatorVariableSize {
    unsigned char       _pad00[0xc14];
    unsigned long       m_usage;                   // +0xc14: the header size
    unsigned long       m_alignmentShift;          // +0xc18
    SVarChunk_00a65330* m_pFirstChunk;             // +0xc1c
    void Validate();
};

void CMemoryAllocatorVariableSize::Validate()
{
    unsigned long shift = m_alignmentShift;
    g_LastAllocatorChunkCounter_013d28d8 = shift;
    unsigned long usage = m_usage;
    g_LastAllocatorUsage_013d28d4 = usage;
    for (SVarChunk_00a65330* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) {
        for (SArea_00a65330* area = chunk->m_pFirstArea; area; area = area->m_pNext)
            g_pGlobalAllocator_013caa90->ValidateBlock((char*)area + (((g_LastAllocatorUsage_013d28d4 + 0x1f) >> g_LastAllocatorChunkCounter_013d28d8) << g_LastAllocatorChunkCounter_013d28d8));
    }
}
