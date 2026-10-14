// CMemoryAllocatorVariableSize::GetAllocationSize(void* block) -- publish the two usage counters to the statistics globals, then read the size (+0x18) of the area header that precedes the block by the aligned header size (`(usage + 0x1f) >> shift << shift`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A651B0, length 48 bytes.
// Leaf: GetAllocationSize
// Shape: `this` moved to eax so the shift count sits in cl, `ret 4`.
struct SAreaHeader_00a651b0 {
    unsigned char _pad00[0x18];
    long          m_size;                          // +0x18
};
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

struct CMemoryAllocatorVariableSize {
    unsigned char _pad00[0xc14];
    unsigned long m_usage;                         // +0xc14: the header size
    unsigned long m_alignmentShift;                // +0xc18
    long GetAllocationSize(void* block);
};

long CMemoryAllocatorVariableSize::GetAllocationSize(void* block)
{
    unsigned long shift = m_alignmentShift;
    g_LastAllocatorChunkCounter_013d28d8 = shift;
    unsigned long usage = m_usage;
    g_LastAllocatorUsage_013d28d4 = usage;
    SAreaHeader_00a651b0* header = (SAreaHeader_00a651b0*)((char*)block - (((usage + 0x1f) >> shift) << shift));
    return header->m_size;
}