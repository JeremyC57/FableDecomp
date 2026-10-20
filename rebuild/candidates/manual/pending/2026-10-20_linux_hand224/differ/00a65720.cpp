// CMemoryAllocatorVariableSize::NewChunk(long) -- reserve and commit a new chunk (halving the request until VirtualAlloc succeeds) and link it at the head of the chunk list
// Grade: DIFFER(ours v retail) -- register assignment: minSize is kept in ebp (retail re-reads it from the stack and keeps regionSize in ebp, re-zeroed with `xor ebp,ebp ; push ebp` as VirtualAlloc's null address), so the alignment globals and the VirtualQuery buffer address use other registers
struct CMemoryAreaChunk;
struct CMemoryAllocatorVariableSize;
// Process-wide copies of the active allocator's alignment (0x013D28D4 alignment, 0x013D28D8 alignment shift).
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
    void Init(CMemoryAreaChunk* owner, long size) {
        PNextArea = 0; PRightChild = 0; PPreviousArea = 0; PParentArea = 0; PLeftChild = 0;
        Colour = 0; PParentChunk = owner; DataSize = size;
    }
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
    void RemoveAreaFromFreeList(CMemoryArea* area);   // 0x00A65060
    void InsertAreaIntoFreeList(CMemoryArea* area);   // 0x00A650F0
    void Init(void* memory, long size, CMemoryAllocatorVariableSize* parent);   // 0x00A65410
    CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size);
};
struct MEMORY_BASIC_INFORMATION {               // winnt.h, 0x1c bytes
    void*         BaseAddress;
    void*         AllocationBase;
    unsigned long AllocationProtect;
    unsigned long RegionSize;                   // +0x0c
    unsigned long State;
    unsigned long Protect;
    unsigned long Type;
};
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* address, unsigned long size, unsigned long type, unsigned long protect);   // [0x0143FD44]
extern "C" __declspec(dllimport) unsigned long __stdcall VirtualQuery(const void* address, MEMORY_BASIC_INFORMATION* info, unsigned long length);   // [0x0143FE80]
struct CMemoryManager {
    void OnOutOfMemory();                       // 0x009F1F00 (empty in retail)
};
extern CMemoryManager* g_pMemoryManager;        // 0x013CAA90
struct CMemoryAllocatorVariableSize {
    unsigned char     _pad000[0xc14];
    unsigned long     Alignment;                // +0xc14
    unsigned long     AlignmentShift;           // +0xc18
    CMemoryAreaChunk* PFirstChunk;              // +0xc1c
    long              MinChunkSize;             // +0xc20
    long              NoFreesSinceConsolidation;   // +0xc24
    unsigned long     Flags;                    // +0xc28 (bit 1: read-only pages)
    unsigned long     InternalFlags;            // +0xc2c
    unsigned long     ExtraMemUsedByOS;         // +0xc30
    CMemoryArea*      PTreeRoot;                // +0xc34
    CMemoryAreaChunk* NewChunk(long minSize);
};

CMemoryAreaChunk* CMemoryAllocatorVariableSize::NewChunk(long minSize)
{
    unsigned long size = ALIGNED_SIZE(sizeof(CMemoryAreaChunk)) + minSize + ALIGNED_SIZE(sizeof(CMemoryArea));
    if (size <= (unsigned long)MinChunkSize)
        size = MinChunkSize;
    unsigned long regionSize = 0;
    long allocSize = ((Alignment + size - 1) >> AlignmentShift) << AlignmentShift;
    void* memory = 0;
    while (memory == 0 && allocSize >= minSize) {
        memory = VirtualAlloc(0, allocSize, 0x103000, ((unsigned char)Flags & 2) ? 2 : 4);
        if (memory) {
            MEMORY_BASIC_INFORMATION info;
            VirtualQuery(memory, &info, sizeof(info));
            regionSize = info.RegionSize;
        }
        allocSize /= 2;
    }
    if (!memory) {
        g_pMemoryManager->OnOutOfMemory();
        return 0;
    }
    CMemoryAreaChunk* chunk = (CMemoryAreaChunk*)memory;
    chunk->Init((char*)memory + ALIGNED_SIZE(sizeof(CMemoryAreaChunk)), regionSize - ALIGNED_SIZE(sizeof(CMemoryAreaChunk)), this);
    chunk->PNextChunk = PFirstChunk;
    if (PFirstChunk)
        PFirstChunk->PPreviousChunk = chunk;
    chunk->PPreviousChunk = 0;
    PFirstChunk = chunk;
    return chunk;
}
