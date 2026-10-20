// CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea*, long) -- carve an allocated area out of a free area
// Grade: DIFFER(ours v retail) -- register/scheduling only: the size argument is loaded into edx before `push esi` and the header size computed in esi (retail: header in eax, size into ecx, `lea esi,[eax+ecx]`), the second Init's size is re-read late through eax (retail edx early), and the allocated-list loads rotate eax/ecx/edx
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

CMemoryArea* CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea* area, long size)
{
    long needed = ALIGNED_SIZE(sizeof(CMemoryArea)) + size;
    __int64 checked = needed;
    if (checked < 0 || checked > 0x7FFFFFFF)
        return 0;
    if (area->DataSize > needed) {
        RemoveAreaFromFreeList(area);
        area->Init(this, area->DataSize - needed);
        CMemoryArea* allocated = (CMemoryArea*)((char*)area + ALIGNED_SIZE(sizeof(CMemoryArea)) + area->DataSize);
        allocated->Init(this, size);
        InsertAreaIntoFreeList(area);
        allocated->PNextArea = PFirstAllocatedArea;
        allocated->PPreviousArea = 0;
        if (PFirstAllocatedArea)
            PFirstAllocatedArea->PPreviousArea = allocated;
        ++NoAllocatedAreas;
        PFirstAllocatedArea = allocated;
        return allocated;
    }
    RemoveAreaFromFreeList(area);
    area->PNextArea = PFirstAllocatedArea;
    area->PPreviousArea = 0;
    if (PFirstAllocatedArea)
        PFirstAllocatedArea->PPreviousArea = area;
    ++NoAllocatedAreas;
    PFirstAllocatedArea = area;
    return area;
}
