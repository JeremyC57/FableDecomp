// CMemoryAllocatorVariableSize::Allocate(unsigned long) -- allocate a block: try the existing chunks, else add a chunk and retry; returns the data start past the area header or 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A659C0, 144 bytes (padding included).
// Grade: DIFFER(140v140) -- scheduling only: after the global stores retail reloads Alignment (+0xc14)
//        into edx before AlignmentShift (+0xc18) into ecx, ours loads ecx first; everything else matches
// Shape: the alignment globals (0x013D28D8 shift, 0x013D28D4 alignment) are refreshed, then the first
// attempt is an inline copy of AllocateArea (the out-of-line body at 0x00A656E0: round the size up with
// the members, AllocateMemoryFromExistingChunks 0x00A64C10, the found area's chunk carves it with
// AllocAreaFromArea 0x00A65480). On failure NewChunk(size) (0x00A65720) and the out-of-line
// AllocateArea(size) retry; the result is CMemoryArea::GetDataStart (area + ALIGNED_SIZE(32) from the
// globals). AllocateArea is an in-class inline: VC7.1 inlines the first call site only and keeps the
// retry as a call (exactly retail's split). The success return is reached through one `if (area)`.
struct CMemoryAreaChunk;
struct CMemoryAllocatorVariableSize;

// Process-wide copies of the active allocator's alignment, refreshed on entry to the allocator's
// public methods (0x013D28D4 alignment, 0x013D28D8 alignment shift).
extern unsigned long g_CurrentAlignment;        // 0x013D28D4
extern unsigned long g_CurrentAlignmentShift;   // 0x013D28D8

// Round a header size up to the current alignment: ((size + align - 1) >> shift) << shift.
#define ALIGNED_SIZE(size) ((((size) + g_CurrentAlignment - 1) >> g_CurrentAlignmentShift) << g_CurrentAlignmentShift)

struct CMemoryArea {                            // PDB: 32 bytes
    CMemoryArea*      PNextArea;                // +0x00
    CMemoryArea*      PRightChild;              // +0x04
    CMemoryArea*      PPreviousArea;            // +0x08
    CMemoryArea*      PParentArea;              // +0x0c
    CMemoryArea*      PLeftChild;               // +0x10
    CMemoryAreaChunk* PParentChunk;             // +0x14
    long              DataSize;                 // +0x18
    unsigned char     Colour;                   // +0x1c
    unsigned char     ID;                       // +0x1d
};

struct CMemoryAreaChunk {                       // PDB: 52 bytes
    long              TotalDataSize;            // +0x00
    long              NoFreeAreas;              // +0x04
    long              NoAllocatedAreas;         // +0x08
    long              FreeDataSize;             // +0x0c
    long              LargestFreeAreaSize;      // +0x10
    long              SmallestFreeAreaSize;     // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;   // +0x18
    bool              Permanent;                // +0x1c
    bool              DemandPaged;              // +0x1d
    CMemoryAreaChunk* PNextChunk;               // +0x20
    CMemoryAreaChunk* PPreviousChunk;           // +0x24
    CMemoryArea*      PFirstFreeArea;           // +0x28
    CMemoryArea*      PLastFreeArea;            // +0x2c
    CMemoryArea*      PFirstAllocatedArea;      // +0x30
    CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size);   // 0x00A65480
};

struct CMemoryAllocatorBase {                   // PDB: 3100 bytes
    virtual void v00();
    unsigned char     PerformanceStats[0xc10];  // +0x04 CPerformanceStats
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
};

struct CMemoryAllocatorVariableSize : CMemoryAllocatorBase {   // PDB: 3128 bytes, vtable 0x0129DB1C
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;// +0xc24
    unsigned long     Flags;                    // +0xc28
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
    void* Allocate(unsigned long size);
    CMemoryArea* AllocateMemoryFromExistingChunks(unsigned long size);   // 0x00A64C10
    CMemoryAreaChunk* NewChunk(long size);                              // 0x00A65720
    CMemoryArea* AllocateArea(unsigned long size)                    // 0x00A656E0
    {
        unsigned long aligned = ((Alignment + size - 1) >> AlignmentShift) << AlignmentShift;
        CMemoryArea* area = AllocateMemoryFromExistingChunks(aligned);
        if (area)
            return area->PParentChunk->AllocAreaFromArea(area, aligned);
        return 0;
    }
};

static inline void* GetDataStart(CMemoryArea* area)
{
    return (char*)area + ALIGNED_SIZE(sizeof(CMemoryArea));
}

void* CMemoryAllocatorVariableSize::Allocate(unsigned long size)
{
    g_CurrentAlignmentShift = AlignmentShift;
    g_CurrentAlignment = Alignment;
    CMemoryArea* area = AllocateArea(size);
    if (area == 0) {
        if (NewChunk(size) == 0)
            return 0;
        area = AllocateArea(size);
    }
    if (area)
        return GetDataStart(area);
    return 0;
}
