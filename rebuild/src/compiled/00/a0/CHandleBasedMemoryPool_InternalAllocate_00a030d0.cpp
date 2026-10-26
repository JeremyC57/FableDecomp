// CHandleBasedMemoryPool::InternalAllocate(long& handle, unsigned long size) -- round the size up to 16, take a fitting free area off its free list, mark it taken and trim it, account the allocation, refresh the largest-free-area cache when the largest one was used, then hand out a handle slot pointing at the area
// Leaf: InternalAllocate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A030D0, 176 bytes, `ret 8`.
// Callees: CHandleBasedMemoryPool::FindFreeArea(long size) @ 0x00A01ED0, ResizeArea @ 0x00A02410,
//   UpdateLargestFreeArea @ 0x00A01F80, InternalAllocateHandle @ 0x00A03040 (the last three landed).
// Layout: Ego_r.pdb CHandleBasedMemoryPool names; engine/CHandleBasedMemoryPool.h gives Handles a 16-byte CArray, retail's
//   is 12 bytes (landed ValidateResources 0x00A02AD0 / InternalAllocateHandle 0x00A03040), so everything after +0x438
//   sits 4 bytes lower in retail (FirstUnusedHandle +0x444, CriticalSection +0x44c) and `this` is modelled locally.
//   CHeader is the Ego_r.pdb 32-byte record; CHandleInfo the 4-byte union {Header, NextFree}.
// Shape: the rounding is the unsigned `(size + 15) / 16 * 16` (`shr 4 ; shl 4`); the "was it the largest" test is a bool
//   taken before the free-list unlink (`sete bl`); the unlink is the inline CHeader member shared with MoveArea; the
//   handle is written through the reference and re-read for the area's Handle field; the success path is the `if (area)`
//   body with `return false` after it (an early `if (!area) return false;` places the failure exit first).
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

class CHandleBasedMemoryPool { public:
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
    CHeader* FindFreeArea(long size);              // 0x00A01ED0
    void ResizeArea(CHeader* area, long newSize);  // 0x00A02410
    void UpdateLargestFreeArea();                  // 0x00A01F80
    long InternalAllocateHandle();                 // 0x00A03040
    bool InternalAllocate(long& handle, unsigned long size);
};

bool CHandleBasedMemoryPool::InternalAllocate(long& handle, unsigned long size)
{
    size = (size + 15) / 16 * 16;
    CHeader* area = FindFreeArea(size);
    if (area) {
        bool wasLargest = area->Size == LargestFreeArea;
        area->RemoveFromFreeList();
        area->Status = 'T';
        ResizeArea(area, size);
        ++NumAllocations;
        AllocatedSize += area->Padding + area->Size + 0x20;
        if (wasLargest)
            UpdateLargestFreeArea();
        handle = InternalAllocateHandle();
        Handles[handle].Header = area;
        area->Handle = handle;
        return true;
    }
    return false;
}