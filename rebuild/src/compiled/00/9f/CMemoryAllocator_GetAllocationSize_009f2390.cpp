// CMemoryAllocator::GetAllocationSize(void* block) -- under the allocator lock, find which sub-allocator owns a tracked block (blocks inside the reserved region encode the ID in bits 24+ of their offset, others carry it in the byte at block - 5) and ask it for the size of the area (block - 4); 0 for a null block or an unknown ID (both early returns leave the lock held, as retail does)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2390, length 110 bytes.
// Callees: KERNEL32 EnterCriticalSection / LeaveCriticalSection (IAT 0x0143FE0C / 0x0143FE10) on the lock 0x013CAA70, the sub-allocator's virtual slot 3 (GetAllocationSize).
// Globals: reserved region bounds 0x013CAAA4 / 0x013CAAA8; CAllocatorInfo table (PAllocator of entry 0 at 0x01396880, 0x1c stride).
// Layout (PDB CMemoryAllocator): NoAllocators +0x410.
// Shape: the region offset is taken as an UNSIGNED long before the shift (`shr`, a pointer difference gives `sar`); the ID is an
//   inline returning `unsigned char` (`movzx eax,al` before the compare).
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // IAT 0x0143FE10
extern unsigned char g_allocatorLock[24];   // 0x013CAA70
extern unsigned char* g_pRegionStart;       // 0x013CAAA4
extern unsigned char* g_pRegionEnd;         // 0x013CAAA8
struct CMemoryAllocatorBase {
    virtual void v0(); virtual void v1(); virtual void v2();
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
};
unsigned long CMemoryAllocator::GetAllocationSize(void* block)
{
    EnterCriticalSection(g_allocatorLock);
    if (block == 0)
        return 0;
    unsigned long id = GetAllocatorID(block);
    if (id >= NoAllocators)
        return 0;
    unsigned long size = g_AllocatorInfo[id].PAllocator->GetAllocationSize((unsigned char*)block - 4);
    LeaveCriticalSection(g_allocatorLock);
    return size;
}