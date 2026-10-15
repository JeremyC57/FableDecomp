// CMemoryAllocatorVariableSize::CMemoryAllocatorVariableSize(void*, long, void*, long, unsigned long, unsigned long, unsigned long) -- variable-size allocator ctor: base with the alignment, settings stored, empty chunk list/tree, optional permanent first chunk (caller block or fresh)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65930, 144 bytes (padding included).
// Shape: CMemoryAllocatorBase(alignment) (0x00A65AD0); all members are initialisers in declaration
// order: VC7.1 stores the argument-valued ones (MinChunkSize/Flags/InternalFlags) before the vtable
// 0x0129DB1C and the zero ones (PFirstChunk/PTreeRoot, one shared zero register) after it; as body
// assignments the scheduling differs. The alignment globals (0x013D28D8 shift, 0x013D28D4 alignment)
// are refreshed in the body. With a non-zero
// initial size the first chunk is made from the caller's block (NewChunkDemandPaged 0x00A65830) or
// allocated (NewChunk 0x00A65720) and marked Permanent (+0x1c); the size is pushed once for both arms.
// The first argument is not read by this body.
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
};

struct CMemoryAllocatorBase {                   // PDB: 3100 bytes
    virtual void v00();
    unsigned char     PerformanceStats[0xc10];  // +0x04 CPerformanceStats
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
    CMemoryAllocatorBase(unsigned long alignment);   // 0x00A65AD0
};

struct CMemoryAllocatorVariableSize : CMemoryAllocatorBase {   // PDB: 3128 bytes, vtable 0x0129DB1C
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;// +0xc24
    unsigned long     Flags;                    // +0xc28
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
    CMemoryAllocatorVariableSize(void* name, long minChunkSize, void* initialMemory, long initialSize,
                                 unsigned long alignment, unsigned long flags, unsigned long internalFlags);
    CMemoryAreaChunk* NewChunk(long size);                              // 0x00A65720
    CMemoryAreaChunk* NewChunkDemandPaged(void* memory, long size);     // 0x00A65830
};

CMemoryAllocatorVariableSize::CMemoryAllocatorVariableSize(void* name, long minChunkSize, void* initialMemory, long initialSize,
                                                           unsigned long alignment, unsigned long flags, unsigned long internalFlags)
    : CMemoryAllocatorBase(alignment)
    , PFirstChunk(0)
    , MinChunkSize(minChunkSize)
    , Flags(flags)
    , InternalFlags(internalFlags)
    , PTreeRoot(0)
{
    g_CurrentAlignmentShift = AlignmentShift;
    g_CurrentAlignment = Alignment;
    if (initialSize != 0) {
        if (initialMemory != 0)
            NewChunkDemandPaged(initialMemory, initialSize)->Permanent = true;
        else
            NewChunk(initialSize)->Permanent = true;
    }
}