// CMemoryAreaChunk::Init(void*, long, CMemoryAllocatorVariableSize*) -- set up a chunk over a raw block: clear the lists and counters, turn the whole block into one free area and file it in the free list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65410, 112 bytes (padding included).
// Shape: the parent pointer first, then the list heads (+0x20..+0x30) and counters (+4, +8, +0x10,
// +0x14) zeroed from one register; the first CMemoryArea sits at the start of the block and gets
// size - ALIGNED_SIZE(sizeof(CMemoryArea)) (the `add 1Fh ; shr cl ; shl cl` rounding with the
// alignment globals 0x013D28D4/0x013D28D8); TotalDataSize takes that size, FreeDataSize is 0 until
// InsertAreaIntoFreeList (0x00A650F0) adds the area. Layouts from the PDB (CMemoryAreaChunk 52 bytes,
// CMemoryArea 32 bytes).
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
    void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);
    void InsertAreaIntoFreeList(CMemoryArea* area);   // 0x00A650F0
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
};

void CMemoryAreaChunk::Init(void* memory, long size, CMemoryAllocatorVariableSize* parent)
{
    PParentAllocator = parent;
    PNextChunk = 0;
    PPreviousChunk = 0;
    PFirstFreeArea = 0;
    PLastFreeArea = 0;
    PFirstAllocatedArea = 0;
    NoFreeAreas = 0;
    NoAllocatedAreas = 0;
    LargestFreeAreaSize = 0;
    SmallestFreeAreaSize = 0;
    long dataSize = size - ALIGNED_SIZE(sizeof(CMemoryArea));
    CMemoryArea* area = (CMemoryArea*)memory;
    area->PNextArea = 0;
    area->PRightChild = 0;
    area->PPreviousArea = 0;
    area->PParentArea = 0;
    area->PLeftChild = 0;
    area->Colour = 0;
    area->PParentChunk = this;
    area->DataSize = dataSize;
    TotalDataSize = dataSize;
    FreeDataSize = 0;
    InsertAreaIntoFreeList(area);
}