// CMemoryAllocator::InternalDeallocate(void* block) -- release a tracked block: find its sub-allocator by ID, take its byte count off the per-tracking-ID total (from the debug info at the end of the block), fill the area with 0xCD, hand it back to the sub-allocator and count one allocation fewer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2920, length 155 bytes.
// Callees: CMemoryAllocator::GetAllocationSize @ 0x009F2390 (twice, the first result discarded -- the same inline debug-info accessor as the
//   landed InitDebugInfo 0x009F28D0), the sub-allocator's virtual slots 3 (GetAllocationSize) and 1 (Deallocate).
// Globals: reserved region bounds 0x013CAAA4 / 0x013CAAA8; CAllocatorInfo table (PAllocator of entry 0 at 0x01396880).
// Layout (PDB CMemoryAllocator): MemoryAllocatedPerTrackingID[256] +0, NoAllocations +0x40c, NoAllocators +0x410; the debug info
//   (guard, tracking ID, size) is the 12 bytes at block + size - 0x14.
// Shape: the area (block - 4) is computed up front into ebp; the fill is the memset intrinsic (`rep stosd ; rep stosb` with 0xCDCDCDCD).
#include <string.h>
extern unsigned char* g_pRegionStart;       // 0x013CAAA4
extern unsigned char* g_pRegionEnd;         // 0x013CAAA8
struct CMemoryAllocatorBase {
    virtual void* Allocate(unsigned long size);
    virtual void Deallocate(void* area);
    virtual void v2();
    virtual unsigned long GetAllocationSize(void* area);
};
struct CAllocatorInfo {
    long MaxAllocSize;
    long MinChunkSize;
    long InitialSize;
    long Alignment;
    unsigned long Flags;
    unsigned long InternalFlags;
    CMemoryAllocatorBase* PAllocator;
};
extern CAllocatorInfo g_AllocatorInfo[];   // 0x01396868
struct SDebugInfo {
    long          Guard;
    unsigned long TrackingID;
    long          Size;
};
__forceinline unsigned char GetAllocatorID(void* block)
{
    if ((unsigned char*)block >= g_pRegionStart && (unsigned char*)block < g_pRegionEnd)
        return (unsigned char)((unsigned long)((unsigned char*)block - g_pRegionStart) >> 24);
    return ((unsigned char*)block)[-5];
}
struct CMemoryAllocator {
    unsigned long MemoryAllocatedPerTrackingID[256];
    unsigned long MemoryAllocated;
    void** MemoryAuditBuffer;
    void* Heap;
    unsigned long NoAllocations;
    unsigned long NoAllocators;
    unsigned long GetAllocationSize(void* block);
    __forceinline SDebugInfo* GetDebugInfo(void* block)
    {
        GetAllocationSize(block);
        return (SDebugInfo*)((unsigned char*)block + GetAllocationSize(block) - 0x14);
    }
    void InternalDeallocate(void* block);
};
void CMemoryAllocator::InternalDeallocate(void* block)
{
    void* area = (unsigned char*)block - 4;
    unsigned long id = GetAllocatorID(block);
    if (id < NoAllocators) {
        CMemoryAllocatorBase* allocator = g_AllocatorInfo[id].PAllocator;
        SDebugInfo* info = GetDebugInfo(block);
        MemoryAllocatedPerTrackingID[info->TrackingID] -= info->Size;
        memset(area, 0xcd, allocator->GetAllocationSize(area));
        allocator->Deallocate(area);
        --NoAllocations;
    }
}