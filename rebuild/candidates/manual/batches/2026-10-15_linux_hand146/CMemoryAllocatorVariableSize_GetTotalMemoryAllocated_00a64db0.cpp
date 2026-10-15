// CMemoryAllocatorVariableSize::GetTotalMemoryAllocated() -- publish the alignment globals, then sum over every chunk: its allocated area headers (count x aligned sizeof(CMemoryArea)), its aligned chunk header and the payload sizes of its allocated areas
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64DB0, length 100 bytes. No callees (vtable 0x0129DB1C slot 7).
// Globals: current alignment shift 0x013D28D8, alignment 0x013D28D4.
// Layout (PDB): CMemoryAllocatorVariableSize PFirstChunk +0xc1c; CMemoryAreaChunk (variable-size flavour, 0x34) NoAllocatedAreas +8,
//   PNextChunk +0x20, PFirstAllocatedArea +0x30; CMemoryArea (0x20) next at +0, payload size at +0x18.
// Shape: the alignment helper is STATIC and reads the globals just stored, so VC forwards the stored values (shift in ecx,
//   alignment in esi) instead of re-reading the members; both aligned sizes ((n + align - 1) >> shift << shift) are hoisted out of the loop.
extern unsigned long g_AllocatorAlignmentShift;   // 0x013D28D8
extern unsigned long g_AllocatorAlignment;        // 0x013D28D4

struct CMemoryArea {
    CMemoryArea*  PNext;                           // +0x00 next area on the chunk's list
    unsigned char _pad04[0x14];
    unsigned long Size;                            // +0x18 payload size
    unsigned char _pad1c[4];
};
struct CMemoryAreaChunk {
    long              TotalDataSize;               // +0x00
    long              NoFreeAreas;                 // +0x04
    long              NoAllocatedAreas;            // +0x08
    long              FreeDataSize;                // +0x0c
    long              LargestFreeAreaSize;         // +0x10
    long              SmallestFreeAreaSize;        // +0x14
    void*             PParentAllocator;            // +0x18
    bool              Permanent;                   // +0x1c
    bool              DemandPaged;                 // +0x1d
    CMemoryAreaChunk* PNextChunk;                  // +0x20
    CMemoryAreaChunk* PPreviousChunk;              // +0x24
    CMemoryArea*      PFirstFreeArea;              // +0x28
    CMemoryArea*      PLastFreeArea;               // +0x2c
    CMemoryArea*      PFirstAllocatedArea;         // +0x30
};
struct CMemoryAllocatorVariableSize {
    unsigned char     _base[0xc14];                // vptr + CPerformanceStats
    unsigned long     Alignment;                   // +0xc14
    unsigned long     AlignmentShift;              // +0xc18
    CMemoryAreaChunk* PFirstChunk;                 // +0xc1c
    static __forceinline unsigned long AlignSize(unsigned long size) { return ((size + g_AllocatorAlignment - 1) >> g_AllocatorAlignmentShift) << g_AllocatorAlignmentShift; }
    unsigned long GetTotalMemoryAllocated();
};

unsigned long CMemoryAllocatorVariableSize::GetTotalMemoryAllocated()
{
    g_AllocatorAlignmentShift = AlignmentShift;
    g_AllocatorAlignment = Alignment;
    unsigned long total = 0;
    for (CMemoryAreaChunk* chunk = PFirstChunk; chunk; chunk = chunk->PNextChunk) {
        unsigned long data = 0;
        for (CMemoryArea* area = chunk->PFirstAllocatedArea; area; area = area->PNext)
            data += area->Size;
        total += chunk->NoAllocatedAreas * AlignSize(sizeof(CMemoryArea)) + AlignSize(sizeof(CMemoryAreaChunk)) + data;
    }
    return total;
}
