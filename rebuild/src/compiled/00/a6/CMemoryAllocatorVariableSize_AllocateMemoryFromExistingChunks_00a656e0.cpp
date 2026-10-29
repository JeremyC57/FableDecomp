// CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(unsigned long size) -- align the request, find the smallest free area in the size tree that holds it and carve the allocation out of that area's chunk; 0 when no existing chunk has room
// Leaf: AllocateMemoryFromExistingChunks
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A656E0, 59 bytes, `ret 4` (PDB ?AllocateMemoryFromExistingChunks@CMemoryAllocatorVariableSize@@QAEPAVCMemoryArea@1@K@Z).
// Callees: CMemoryAllocatorVariableSize::GetTreeAreaWithSizeGreaterThanOrEqualTo(long) const @ 0x00A64C10 (the manifest
//   mislabels it AllocateMemoryFromExistingChunks: it is the best-fit walk from PTreeRoot +0xc34),
//   CMemoryAreaChunk::AllocAreaFromArea(CMemoryArea*, long) @ 0x00A65480. Caller: CMemoryAllocatorVariableSize::Allocate
//   @ 0x00A659C0 (vtable 0x0129DB1C slot 0), which inlines this body once and calls it after making a new chunk.
// Label: the manifest's `MemoryAllocator_AllocateFromPool` is descriptive.
// Layout: engine/CMemoryAllocatorBase.h (Alignment +0xc14, AlignmentShift +0xc18) as the base of the Ego_r.pdb
//   CMemoryAllocatorVariableSize; CMemoryArea::PParentChunk +0x14.
// Shape: CMemoryAllocatorBase::Align inline (`(size + Alignment - 1) >> AlignmentShift << AlignmentShift`, `this` moved
//   to eax so ecx can carry the shift count); the aligned size is reused for both calls.
#include "engine/CMemoryAllocatorBase.h"

class CMemoryAllocatorVariableSize : public CMemoryAllocatorBase {
public:
    class CMemoryArea;
    class CMemoryAreaChunk {
    public:
        CMemoryArea* AllocAreaFromArea(CMemoryArea* area, long size);   // 0x00A65480
    };
    class CMemoryArea {
    public:
        CMemoryArea*      PNextArea;                    // +0x00
        CMemoryArea*      PRightChild;                  // +0x04
        CMemoryArea*      PPreviousArea;                // +0x08
        CMemoryArea*      PParentArea;                  // +0x0c
        CMemoryArea*      PLeftChild;                   // +0x10
        CMemoryAreaChunk* PParentChunk;                 // +0x14
        long              DataSize;                     // +0x18
        unsigned char     Colour;                       // +0x1c
        unsigned char     ID;                           // +0x1d
    };

    CMemoryAreaChunk* PFirstChunk;                      // +0xc1c
    long              MinChunkSize;                     // +0xc20
    long              NoFreesSinceConsolidation;        // +0xc24
    unsigned long     Flags;                            // +0xc28
    unsigned long     InternalFlags;                    // +0xc2c
    unsigned long     ExtraMemUsedByOS;                 // +0xc30
    CMemoryArea*      PTreeRoot;                        // +0xc34

    unsigned long Align(unsigned long size) const { return (size + Alignment - 1) >> AlignmentShift << AlignmentShift; }
    protected: CMemoryArea* GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const; public:   // 0x00A64C10
    CMemoryArea* AllocateMemoryFromExistingChunks(unsigned long size);
};

CMemoryAllocatorVariableSize::CMemoryArea* CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(unsigned long size)
{
    size = Align(size);
    CMemoryArea* area = GetTreeAreaWithSizeGreaterThanOrEqualTo(size);
    if (area)
        return area->PParentChunk->AllocAreaFromArea(area, size);
    return 0;
}