// CMemoryAreaChunk::InsertAreaIntoFreeList(CMemoryArea* area) -- insert a free area into the chunk's size-sorted free list, add it to the allocator's free-area tree, update the free size, largest/smallest free sizes and the free-area count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A650F0, length 188 bytes (`ret 4`).
// Callees: CMemoryAllocatorVariableSize::InsertIntoTree(CMemoryArea* root, CMemoryArea* area) @ 0x00A64E70 (on the parent allocator).
// Layout (Ego_r.pdb, retail-identical): CMemoryAreaChunk NoFreeAreas +0x04, FreeDataSize +0x0C, LargestFreeAreaSize +0x10,
// SmallestFreeAreaSize +0x14, PParentAllocator +0x18, PFirstFreeArea +0x28, PLastFreeArea +0x2C; CMemoryArea PNextArea +0x00,
// PPreviousArea +0x08, DataSize +0x18; CMemoryAllocatorVariableSize::PTreeRoot +0xC34. No engine header for these classes.
// Shape: an empty list takes the area as first and last; otherwise a walk that inserts before the first larger area or appends at the
// tail (both exits `break` out of the loop, the tail store shared with the empty case).
struct CMemoryAreaChunk;
struct CMemoryArea {
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
struct CMemoryAllocatorVariableSize {
    unsigned char _base[0xc34];
    CMemoryArea*  PTreeRoot;                    // +0xc34
    void InsertIntoTree(CMemoryArea* root, CMemoryArea* area);   // 0x00A64E70
};
struct CMemoryAreaChunk {
    long                          TotalDataSize;        // +0x00
    long                          NoFreeAreas;          // +0x04
    long                          NoAllocatedAreas;     // +0x08
    long                          FreeDataSize;         // +0x0c
    long                          LargestFreeAreaSize;  // +0x10
    long                          SmallestFreeAreaSize; // +0x14
    CMemoryAllocatorVariableSize* PParentAllocator;     // +0x18
    bool                          Permanent;            // +0x1c
    bool                          DemandPaged;          // +0x1d
    CMemoryAreaChunk*             PNextChunk;           // +0x20
    CMemoryAreaChunk*             PPreviousChunk;       // +0x24
    CMemoryArea*                  PFirstFreeArea;       // +0x28
    CMemoryArea*                  PLastFreeArea;        // +0x2c
    CMemoryArea*                  PFirstAllocatedArea;  // +0x30
    void InsertAreaIntoFreeList(CMemoryArea* area);
};

void CMemoryAreaChunk::InsertAreaIntoFreeList(CMemoryArea* area)
{
    CMemoryArea* p = PFirstFreeArea;
    if (!p) {
        area->PNextArea = 0;
        area->PPreviousArea = 0;
        PFirstFreeArea = area;
        PLastFreeArea = area;
    }
    else {
        while (p) {
            if (p->DataSize > area->DataSize) {
                area->PNextArea = p;
                area->PPreviousArea = p->PPreviousArea;
                if (p->PPreviousArea) {
                    p->PPreviousArea->PNextArea = area;
                    p->PPreviousArea = area;
                }
                else {
                    PFirstFreeArea = area;
                    p->PPreviousArea = area;
                }
                break;
            }
            if (!p->PNextArea) {
                area->PNextArea = 0;
                area->PPreviousArea = p;
                p->PNextArea = area;
                PLastFreeArea = area;
                break;
            }
            p = p->PNextArea;
        }
    }
    PParentAllocator->InsertIntoTree(PParentAllocator->PTreeRoot, area);
    FreeDataSize += area->DataSize;
    if (PLastFreeArea)
        LargestFreeAreaSize = PLastFreeArea->DataSize;
    else
        LargestFreeAreaSize = 0;
    if (PFirstFreeArea)
        SmallestFreeAreaSize = PFirstFreeArea->DataSize;
    else
        SmallestFreeAreaSize = 0;
    ++NoFreeAreas;
}
