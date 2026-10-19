// Grade: DIFFER(ours v retail) -- 201v201: only the final `mov [esi+444h],ebx` (FirstUnusedHandle = handle) is scheduled after the MultiThreaded reload and the pops in our build; retail stores it first. Scheduling only (tried inline BeginCritical/EndCritical and an inline ReleaseHandle).
// CHandleBasedMemoryPool::Free(long handle) -- under the pool lock: mark the handle's area free, put it on its free list, account the allocation change, merge it with free physical neighbours (next, then previous) and push the handle slot onto the free-handle chain
// Leaf: Free
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A02B30, 201 bytes, `ret 4`.
// Callees: EnterCriticalSection / LeaveCriticalSection through their imports (0x0143FE0C / 0x0143FE10) when MultiThreaded,
//   CHandleBasedMemoryPool::InsertFreeArea @ 0x00A01DA0, CHandleBasedMemoryPool::CombineAreas(CHeader* first, CHeader*
//   second) @ 0x00A01E40 (both landed).
// Layout: Ego_r.pdb CHandleBasedMemoryPool names; engine/CHandleBasedMemoryPool.h gives Handles a 16-byte CArray, retail's
//   is 12 bytes (landed ValidateResources 0x00A02AD0 / InternalAllocateHandle 0x00A03040), so everything after +0x438
//   sits 4 bytes lower in retail (FirstUnusedHandle +0x444, CriticalSection +0x44c) and `this` is modelled locally.
//   CHeader is the Ego_r.pdb 32-byte record; CHandleInfo the 4-byte union {Header, NextFree}.
// Shape: the handle slot's byte offset kept in ebp (`lea ebp,[ebx*4]`); the sizes are read back after InsertFreeArea
//   (which folded Padding into Size); NumAllocations is bumped on free as on allocation (retail behaviour); the slot is
//   cleared, then re-addressed as a CHandleInfo reference that is cleared again and given the old chain head.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // 0x0143FE10

struct CHeader {                                   // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    void*     Resource;                            // +0x00
    long      Handle;                              // +0x04
    long      Size;                                // +0x08
    short     Padding;                             // +0x0c
    unsigned char Status;                          // +0x0e ('N' free, 'T' taken)
    unsigned char Locked;                          // +0x0f
    CHeader** RefAvailable;                        // +0x10 (the link that points at this free area)
    CHeader*  NextAvailable;                       // +0x14
    CHeader*  PrevContiguous;                      // +0x18
    CHeader*  NextContiguous;                      // +0x1c
    void RemoveFromFreeList()
    {
        if (RefAvailable && Status == 'N') {
            Status = 0;
            *RefAvailable = NextAvailable;
            if (NextAvailable)
                NextAvailable->RefAvailable = RefAvailable;
            RefAvailable = 0;
            NextAvailable = 0;
        }
    }
};

union CHandleInfo {                                // CHandleBasedMemoryPool::CHandleInfo (4 bytes): the area, or the next free slot
    CHeader* Header;
    long     NextFree;
};

struct SCriticalSection {                          // RTL_CRITICAL_SECTION (24 bytes)
    void* DebugInfo;
    long  LockCount;
    long  RecursionCount;
    void* OwningThread;
    void* LockSemaphore;
    unsigned long SpinCount;
};

struct CHandleBasedMemoryPool {
    virtual void v00();
    CHeader*      Buffer;                          // +0x04
    void*         UnalignedBuffer;                 // +0x08
    long          PoolSize;                        // +0x0c
    long          AllocatedSize;                   // +0x10
    long          NumAllocations;                  // +0x14
    long          LockCount;                       // +0x18
    long          LargestFreeArea;                 // +0x1c
    unsigned long MemoryFlags;                     // +0x20
    bool          MultiThreaded;                   // +0x24
    bool          ShutdownFlag;                    // +0x25
    bool          EnableDefrag;                    // +0x26
    bool          EnableFullDefragIfPartialFails;  // +0x27
    unsigned long MaxFreeSizeBeforeDefrag;         // +0x28
    unsigned long MaxFreeSizeBeforeFailure;        // +0x2c
    long          MaxBlocksToTryOnPartialDefrag;   // +0x30
    void*         FailureHandler;                  // +0x34
    CHeader*      FreeLists[256];                  // +0x38
    CHandleInfo*  Handles;                         // +0x438 CArray<CHandleInfo> (12 bytes in retail: first, last, end)
    CHandleInfo*  HandlesLast;                     // +0x43c
    CHandleInfo*  HandlesEnd;                      // +0x440
    long          FirstUnusedHandle;               // +0x444
    void*         Name;                            // +0x448 CCharString
    SCriticalSection CriticalSection;              // +0x44c
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void CombineAreas(CHeader* first, CHeader* second);   // 0x00A01E40
    void Free(long handle);
};

void CHandleBasedMemoryPool::Free(long handle)
{
    if (MultiThreaded)
        EnterCriticalSection(&CriticalSection);
    CHeader* area = Handles[handle].Header;
    area->Status = 'N';
    area->Locked = 0;
    InsertFreeArea(area);
    ++NumAllocations;
    AllocatedSize -= area->Padding + area->Size + 0x20;
    if (area->NextContiguous && area->NextContiguous->Status == 'N')
        CombineAreas(area, area->NextContiguous);
    if (area->PrevContiguous && area->PrevContiguous->Status == 'N')
        CombineAreas(area->PrevContiguous, area);
    Handles[handle].Header = 0;
    CHandleInfo& slot = Handles[handle];
    slot.Header = 0;
    slot.NextFree = FirstUnusedHandle;
    FirstUnusedHandle = handle;
    if (MultiThreaded)
        LeaveCriticalSection(&CriticalSection);
}
