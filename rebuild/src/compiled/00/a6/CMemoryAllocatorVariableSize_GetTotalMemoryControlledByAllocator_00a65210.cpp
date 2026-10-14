// CMemoryAllocatorVariableSize::GetTotalMemoryControlledByAllocator() -- publish the two usage counters to the statistics globals, then sum over the chunks (+0xc1c, next at +0x20) their size (+0) plus the two aligned header sizes (`(usage + 0x33)` and `(usage + 0x1f)`, each `>> shift << shift`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65210, length 80 bytes.
// Leaf: GetTotalMemoryControlledByAllocator
// Shape: `this` moved to edx so the shift count sits in cl, the header sum computed once in ecx before the aligned loop, `ret`.
struct SVarChunk_00a65210 {
    unsigned long       m_size;                    // +0x00
    unsigned char       _pad04[0x1c];
    SVarChunk_00a65210* m_pNext;                   // +0x20
};
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

struct CMemoryAllocatorVariableSize {
    unsigned char       _pad00[0xc14];
    unsigned long       m_usage;                   // +0xc14: the header size
    unsigned long       m_alignmentShift;          // +0xc18
    SVarChunk_00a65210* m_pFirstChunk;             // +0xc1c
    unsigned long GetTotalMemoryControlledByAllocator();
};

unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryControlledByAllocator()
{
    unsigned long shift = m_alignmentShift;
    g_LastAllocatorChunkCounter_013d28d8 = shift;
    unsigned long usage = m_usage;
    g_LastAllocatorUsage_013d28d4 = usage;
    unsigned long total = 0;
    for (SVarChunk_00a65210* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext)
        total += chunk->m_size + (((usage + 0x33) >> shift) << shift) + (((usage + 0x1f) >> shift) << shift);   // loop-invariant: hoisted after the entry test
    return total;
}