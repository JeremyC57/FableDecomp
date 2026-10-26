// CMemoryAreaChunk::RemoveAreaFromFreeList(CMemoryArea*) -- unlink a free area from the chunk's size-ordered free list and the allocator's tree, then refresh the free counters and the largest/smallest free sizes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65060, 144 bytes (padding included).
// Shape: doubly-linked unlink with the list head/tail (+0x28/+0x2c) as the null-neighbour fallbacks,
// both links cleared, the parent allocator (+0x18) drops the area from its free tree (0x00A64EF0),
// NoFreeAreas-- and FreeDataSize -= area->DataSize, then the tail (largest) and head (smallest) free
// areas give LargestFreeAreaSize/SmallestFreeAreaSize (0 when the list is empty). This is the
// 52-byte PDB CMemoryAreaChunk; the landed 0x00A658E0 is a different (24-byte chunk) allocator.
struct CMemoryAreaChunk;
class CMemoryAllocatorVariableSize;

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
    void RemoveAreaFromFreeList(CMemoryArea* area);
};

struct CMemoryAllocatorBase {                   // PDB: 3100 bytes
    virtual void v00();
    unsigned char     PerformanceStats[0xc10];  // +0x04 CPerformanceStats
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
};

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase { public:   // PDB: 3128 bytes, vtable 0x0129DB1C
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;// +0xc24
    unsigned long     Flags;                    // +0xc28
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
    void RemoveFromTree(CMemoryArea* area);     // 0x00A64EF0
};

void CMemoryAreaChunk::RemoveAreaFromFreeList(CMemoryArea* area)
{
    if (area->PPreviousArea)
        area->PPreviousArea->PNextArea = area->PNextArea;
    else
        PFirstFreeArea = area->PNextArea;
    if (area->PNextArea)
        area->PNextArea->PPreviousArea = area->PPreviousArea;
    else
        PLastFreeArea = area->PPreviousArea;
    area->PNextArea = 0;
    area->PPreviousArea = 0;
    PParentAllocator->RemoveFromTree(area);
    --NoFreeAreas;
    FreeDataSize -= area->DataSize;
    if (PLastFreeArea)
        LargestFreeAreaSize = PLastFreeArea->DataSize;
    else
        LargestFreeAreaSize = 0;
    if (PFirstFreeArea)
        SmallestFreeAreaSize = PFirstFreeArea->DataSize;
    else
        SmallestFreeAreaSize = 0;
}