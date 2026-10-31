// CMemoryAllocatorVariableSize::CMemoryAreaChunk::FreeArea(CMemoryArea* area) -- unlink the area from the chunk's allocated list (area next at +0, prev at +8; the chunk's PFirstAllocatedArea at +0x30), clear its links, decrement NoAllocatedAreas (+8), then re-insert the area into the free list (0x00A650F0) and consolidate its neighbours (0x00A65590)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A658E0, length 80 bytes.
// Leaf: ?FreeArea@CMemoryAreaChunk@CMemoryAllocatorVariableSize@@QAEXPAVCMemoryArea@2@@Z
// Name: Ego_r.pdb + FableWin.pdb CMemoryAreaChunk::FreeArea (public); PDB-build order puts FreeArea (0x00C079E0, 0x50 bytes) right
//   before the CMemoryAllocatorVariableSize ctor (0x00C07A30), as retail 0x00A658E0 (80 bytes) sits before the ctor 0x00A65930.
//   (Landed before naming wave 12 as a second RemoveAreaFromFreeList; 0x00A65060 is RemoveAreaFromFreeList.)
// Callees: InsertAreaIntoFreeList @ 0x00A650F0 (protected), ConsolidateAdjacentFreeAreas @ 0x00A65590 (both thiscall on the chunk with
// the area). Shape: the area in esi, `this` in edi, `ret 4`.
// Layout: Ego_r.pdb (CMemoryArea PNextArea +0x00, PPreviousArea +0x08; CMemoryAreaChunk NoAllocatedAreas +0x08, PFirstAllocatedArea +0x30).
class CMemoryAllocatorVariableSize { public:
    class CMemoryArea;
    class CMemoryAreaChunk;
};
class CMemoryAllocatorVariableSize::CMemoryArea { public:
    CMemoryArea*      PNextArea;                   // +0x00
    CMemoryArea*      PRightChild;                 // +0x04
    CMemoryArea*      PPreviousArea;               // +0x08
};
class CMemoryAllocatorVariableSize::CMemoryAreaChunk { public:
    long              TotalDataSize;               // +0x00
    long              NoFreeAreas;                 // +0x04
    long              NoAllocatedAreas;            // +0x08
    unsigned char     _pad0c[0x24];                // +0x0c
    CMemoryArea*      PFirstAllocatedArea;         // +0x30
    void ConsolidateAdjacentFreeAreas(CMemoryArea* area);   // 0x00A65590
    void FreeArea(CMemoryArea* area);
protected:
    void InsertAreaIntoFreeList(CMemoryArea* area);         // 0x00A650F0
};
typedef CMemoryAllocatorVariableSize::CMemoryArea CMemoryArea;
typedef CMemoryAllocatorVariableSize::CMemoryAreaChunk CMemoryAreaChunk;

void CMemoryAreaChunk::FreeArea(CMemoryArea* area)
{
    if (area->PPreviousArea)
        area->PPreviousArea->PNextArea = area->PNextArea;
    else
        PFirstAllocatedArea = area->PNextArea;
    if (area->PNextArea)
        area->PNextArea->PPreviousArea = area->PPreviousArea;
    area->PNextArea = 0;
    area->PPreviousArea = 0;
    --NoAllocatedAreas;
    InsertAreaIntoFreeList(area);
    ConsolidateAdjacentFreeAreas(area);
}
