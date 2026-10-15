// CMemoryAllocatorVariableSize::NewChunkDemandPaged(void*, long) -- turn a caller-supplied (demand-paged) block into a chunk: header at the start, the rest initialised as one free area, chunk pushed on the front of the chunk list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65830, 108 bytes.
// Shape: the area size is computed into a local BEFORE the DemandPaged (+0x1d) byte store, which
// may alias the alignment globals, so the header size is recomputed for the pointer argument (a
// single expression CSEs it); then CMemoryAreaChunk::Init (0x00A65410) over the block past
// the aligned chunk header (ALIGNED_SIZE(52): `add 33h ; shr cl ; shl cl`, computed once per argument
// because the alignment globals 0x013D28D4/0x013D28D8 are re-read), size minus the same header, and
// this allocator as parent; then the chunk is linked in front of PFirstChunk (+0xc1c, re-read after
// the store) and returned.
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
    void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);   // 0x00A65410
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
    CMemoryAreaChunk* NewChunkDemandPaged(void* memory, long size);
};

CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunkDemandPaged(void* memory, long size)
{
    CMemoryAreaChunk* chunk = (CMemoryAreaChunk*)memory;
    long areaSize = size - ALIGNED_SIZE(sizeof(CMemoryAreaChunk));
    chunk->DemandPaged = true;
    chunk->Init((char*)chunk + ALIGNED_SIZE(sizeof(CMemoryAreaChunk)), areaSize, this);
    chunk->PNextChunk = PFirstChunk;
    if (PFirstChunk)
        PFirstChunk->PPreviousChunk = chunk;
    chunk->PPreviousChunk = 0;
    PFirstChunk = chunk;
    return chunk;
}
