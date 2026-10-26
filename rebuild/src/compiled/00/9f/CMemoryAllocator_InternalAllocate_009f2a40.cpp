// CMemoryAllocator::InternalAllocate(long size, unsigned long type, bool flag) -- allocate a tracked block: clamp the size to >= 1, add the 0x14 bytes of header/debug trailer, pick the size-class allocator of the allocator type (AllocSizeToAllocator[type][min(total, MaxFixedAllocationSizes[type] + 1)]), stamp its ID in the byte before the block when the block lies outside the reserved region, count it, and fill the debug info of the payload (block + 4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2A40, length 122 bytes.
// Callees: the size-class allocator's virtual slot 0 (Allocate), CMemoryAllocator::InitDebugInfo @ 0x009F28D0 (landed).
// Globals: the reserved region bounds 0x013CAAA4 / 0x013CAAA8.
// Layout (PDB CMemoryAllocator): NoAllocations +0x40c, MaxFixedAllocationSizes[15] +0x424, AllocSizeToAllocator*[15] +0x460;
//   CAllocSizeToAllocator (8 bytes) PAllocator +0, ID +4.
// Shape: the clamp runs on the running total itself: `total = size; if ((long)total < 1) total = 1; size = total;
//   total += 0x14;` loads the size into eax before the register pushes, clamps it there with `jge`, copies it to ebp and
//   adds the header in place (retail order). Clamping `size` first (if, ?:, a Max inline) keeps it in ebp and builds the
//   total with `lea eax,[ebp+14h]`; a Max(a, b) `a > b` inline also gives `jg`.
extern unsigned char* g_pRegionStart;       // 0x013CAAA4
extern unsigned char* g_pRegionEnd;         // 0x013CAAA8
struct CMemoryAllocatorBase {
    virtual void* Allocate(unsigned long size);
};
struct CAllocSizeToAllocator {
    CMemoryAllocatorBase* PAllocator;
    unsigned char ID;
};
class CMemoryAllocator { public:
    unsigned long MemoryAllocatedPerTrackingID[256];
    unsigned long MemoryAllocated;
    void** MemoryAuditBuffer;
    void* Heap;
    unsigned long NoAllocations;
    unsigned long NoAllocators;
    unsigned char CurrentTrackingID;
    bool AllocatorExists[15];
    long MaxFixedAllocationSizes[15];
    CAllocSizeToAllocator* AllocSizeToAllocator[15];
    void InitDebugInfo(void* block, bool flag, long size);
    void* InternalAllocate(long size, unsigned long type, bool flag);
};
void* CMemoryAllocator::InternalAllocate(long size, unsigned long type, bool flag)
{
    unsigned long total = size;
    if ((long)total < 1)
        total = 1;
    size = total;
    total += 0x14;
    unsigned long index = total;
    if (total > (unsigned long)(MaxFixedAllocationSizes[type] + 1))
        index = MaxFixedAllocationSizes[type] + 1;
    CAllocSizeToAllocator* entry = &AllocSizeToAllocator[type][index];
    unsigned char* mem = (unsigned char*)entry->PAllocator->Allocate(total);
    if (mem) {
        if (mem < g_pRegionStart || mem >= g_pRegionEnd)
            mem[-1] = entry->ID;
        ++NoAllocations;
        mem += 4;
        InitDebugInfo(mem, flag, size);
    }
    return mem;
}