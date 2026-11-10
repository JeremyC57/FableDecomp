// CMemoryAllocatorVariableSize::Allocate(unsigned long) -- allocate an aligned area, growing the chunk list when needed.
// Leaf: ?Allocate@CMemoryAllocatorVariableSize@@UAEPAXK@Z
// Retail: 0x00a659c0, 140 bytes, virtual thiscall.
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE_ALLOCATE
#include "engine/CMemoryAllocatorVariableSize.h"
// Original best-fit tree walk is visible to this translation unit.
CMemoryAllocatorVariableSize::CMemoryArea*
CMemoryAllocatorVariableSize::GetTreeAreaWithSizeGreaterThanOrEqualTo(long size) const
{
    CMemoryArea* best = 0;
    CMemoryArea* area = PTreeRoot;
    while (area) {
        if (area->DataSize == size) return area;
        if (area->DataSize > size) {
            if (!best || area->DataSize < best->DataSize) best = area;
            area = area->PLeftChild;
        } else {
            area = area->PRightChild;
        }
    }
    return best;
}
void* CMemoryAllocatorVariableSize::Allocate(unsigned long size)
{
    SetCurrentAlignment();
    CMemoryArea* parea = AllocateMemoryFromExistingChunks(size);
    if (!parea) {
        if (NewChunk(size))
            parea = AllocateMemoryFromExistingChunks(size);
    }
    if (parea)
        return parea->GetDataStart();
    return 0;
}