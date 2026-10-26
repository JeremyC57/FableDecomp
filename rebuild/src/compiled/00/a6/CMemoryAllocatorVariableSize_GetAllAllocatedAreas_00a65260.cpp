// CMemoryAllocatorVariableSize::GetAllAllocatedAreas(void**, unsigned long) -- fill an array with the data start of every allocated area of every chunk; returns the count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65260, 112 bytes (padding included).
// Shape: the alignment globals (0x013D28D8 shift, 0x013D28D4 alignment) are refreshed, then a nested
// walk over PFirstChunk (+0xc1c) / PNextChunk (+0x20) and each chunk's PFirstAllocatedArea (+0x30) /
// PNextArea list; each entry is GetDataStart(area) = area + ALIGNED_SIZE(sizeof(CMemoryArea)) computed
// from the globals, which are re-read after every array store (the store may alias them). The
// capacity argument is not checked by this build.
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
    unsigned long GetAllAllocatedAreas(void** areas, unsigned long maxAreas);
};

static inline void* GetDataStart(CMemoryArea* area)
{
    return (char*)area + ALIGNED_SIZE(sizeof(CMemoryArea));
}

unsigned long CMemoryAllocatorVariableSize::GetAllAllocatedAreas(void** areas, unsigned long maxAreas)
{
    g_CurrentAlignmentShift = AlignmentShift;
    g_CurrentAlignment = Alignment;
    unsigned long count = 0;
    for (CMemoryAreaChunk* chunk = PFirstChunk; chunk; chunk = chunk->PNextChunk) {
        for (CMemoryArea* area = chunk->PFirstAllocatedArea; area; area = area->PNextArea)
            areas[count++] = GetDataStart(area);
    }
    return count;
}