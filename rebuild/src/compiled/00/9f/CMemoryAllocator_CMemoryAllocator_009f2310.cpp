// CMemoryAllocator::~CMemoryAllocator() -- tear down the global allocator: free the audit buffer, release every sub-allocator of the allocator-info table, free the fifteen size-to-allocator maps and delete the allocator lock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F2310, length 116 bytes.
// Leaf: ??1CMemoryAllocator
// Callees: free @ 0x00BFEA14, the sub-allocator release @ 0x00A66750 (`__fastcall`, the allocator in ecx; the class operator delete
//   that the landed CMemoryAllocatorFixedSize `??_G` also calls), KERNEL32 DeleteCriticalSection (IAT 0x0143FDE4) on the lock 0x013CAA70.
// Globals: CAllocatorInfo table (0x1c-byte entries) whose PAllocator of entry 0 sits at 0x01396880.
// Layout (PDB CMemoryAllocator, 0x49c, no vtable): MemoryAuditBuffer +0x404, NoAllocators +0x410, AllocSizeToAllocator*[15] +0x460.
// Shape: the info loop re-reads NoAllocators after each call (table walked by ebx += 0x1c); the 15-entry loop becomes a pointer walk with a countdown.
extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void* section);   // IAT 0x0143FDE4
extern "C" void __cdecl free(void* p);                                               // 0x00BFEA14
extern unsigned char g_allocatorLock[24];   // 0x013CAA70
struct CMemoryAllocatorBase;
void __fastcall ReleaseAllocator(CMemoryAllocatorBase* allocator);   // 0x00A66750
struct CAllocatorInfo {
    long MaxAllocSize;
    long MinChunkSize;
    long InitialSize;
    long Alignment;
    unsigned long Flags;
    unsigned long InternalFlags;
    CMemoryAllocatorBase* PAllocator;
};
extern CAllocatorInfo g_AllocatorInfo[];   // 0x01396868 (PAllocator of entry 0 at 0x01396880)
struct CAllocSizeToAllocator;
struct CMemoryAllocator {
    unsigned long          MemoryAllocatedPerTrackingID[256];   // +0x000
    unsigned long          MemoryAllocated;                     // +0x400
    void**                 MemoryAuditBuffer;                   // +0x404
    void*                  Heap;                                // +0x408
    unsigned long          NoAllocations;                       // +0x40c
    unsigned long          NoAllocators;                        // +0x410
    unsigned char          CurrentTrackingID;                   // +0x414
    bool                   AllocatorExists[15];                 // +0x415
    long                   MaxFixedAllocationSizes[15];         // +0x424
    CAllocSizeToAllocator* AllocSizeToAllocator[15];            // +0x460
    ~CMemoryAllocator();
};
CMemoryAllocator::~CMemoryAllocator()
{
    if (MemoryAuditBuffer)
        free(MemoryAuditBuffer);
    for (unsigned long i = 0; i < NoAllocators; ++i)
        ReleaseAllocator(g_AllocatorInfo[i].PAllocator);
    for (int j = 0; j < 15; ++j)
        if (AllocSizeToAllocator[j])
            free(AllocSizeToAllocator[j]);
    DeleteCriticalSection(g_allocatorLock);
}