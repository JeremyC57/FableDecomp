// CMemoryAllocator::InternalAllocate(long size, unsigned long type, bool flag) -- allocate a tracked block: clamp the size to >= 1, add the 0x14 bytes of header/debug trailer, pick the size-class allocator of the allocator type (AllocSizeToAllocator[type][min(total, MaxFixedAllocationSizes[type] + 1)]), stamp its ID in the byte before the block when the block lies outside the reserved region, count it, and fill the debug info of the payload (block + 4)
// Grade: DIFFER(120v122) -- register allocation of the clamp: retail loads the size into eax BEFORE the register pushes, clamps it there (`cmp eax,1 ; jge`), then copies it to ebp (`mov ebp,eax ; add eax,14h`); ours loads it into ebp after the pushes and computes `lea eax,[ebp+14h]`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2A40, length 122 bytes.
// Callees: the size-class allocator's virtual slot 0 (Allocate), CMemoryAllocator::InitDebugInfo @ 0x009F28D0 (landed).
// Globals: the reserved region bounds 0x013CAAA4 / 0x013CAAA8.
// Layout (PDB CMemoryAllocator): NoAllocations +0x40c, MaxFixedAllocationSizes[15] +0x424, AllocSizeToAllocator*[15] +0x460;
//   CAllocSizeToAllocator (8 bytes) PAllocator +0, ID +4.
// Tried: plain `if (size < 1) size = 1;`, `size = size < 1 ? 1 : size;`, a `Max(size, 1L)` inline (all identical: size kept in ebp),
//   `total = Max(size,1) + 0x14; size = total - 0x14;` (loads into eax first but then `jg` and `lea ebp,[eax-14h]`).
extern unsigned char* g_pRegionStart;       // 0x013CAAA4
extern unsigned char* g_pRegionEnd;         // 0x013CAAA8
template <class T> __forceinline T Max(const T& a, const T& b) { return a > b ? a : b; }
struct CMemoryAllocatorBase {
    virtual void* Allocate(unsigned long size);
};
struct CAllocSizeToAllocator {
    CMemoryAllocatorBase* PAllocator;
    unsigned char ID;
};
struct CMemoryAllocator {
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
    size = Max(size, 1L);
    unsigned long total = size + 0x14;
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
