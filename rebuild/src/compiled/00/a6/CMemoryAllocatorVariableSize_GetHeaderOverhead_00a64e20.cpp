// CMemoryAllocatorVariableSize::GetHeaderOverhead() -- publish the two usage counters (+0xc18 / +0xc14) to the statistics globals, then sum over the chunks (+0xc1c, next at +0x20): the area count (+8) times the aligned header size plus the aligned chunk header, both rounded up to the alignment shift held in +0xc18 (`(usage + 0x1f) >> shift << shift` and `(usage + 0x33) >> shift << shift`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64E20, length 80 bytes.
// Leaf: GetHeaderOverhead
// Shape: `this` moved to edx so the shift count sits in cl, the two rounded sizes in esi/edi computed once before the loop, `ret`.
struct SVarChunk_00a64e20 {
    unsigned char       _pad00[8];
    long                m_noAreas;                 // +0x08
    unsigned char       _pad0c[0x14];
    SVarChunk_00a64e20* m_pNext;                   // +0x20
};
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

class CMemoryAllocatorVariableSize { public:
    unsigned char       _pad00[0xc14];
    unsigned long       m_usage;                   // +0xc14: the header size
    unsigned long       m_alignmentShift;          // +0xc18
    SVarChunk_00a64e20* m_pFirstChunk;             // +0xc1c
    unsigned long GetHeaderOverhead();
};

unsigned long CMemoryAllocatorVariableSize::GetHeaderOverhead()
{
    unsigned long shift = m_alignmentShift;
    g_LastAllocatorChunkCounter_013d28d8 = shift;
    unsigned long usage = m_usage;
    g_LastAllocatorUsage_013d28d4 = usage;
    unsigned long total = 0;
    for (SVarChunk_00a64e20* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext)
        total += chunk->m_noAreas * (((usage + 0x1f) >> shift) << shift) + (((usage + 0x33) >> shift) << shift);   // loop-invariant: hoisted after the entry test
    return total;
}