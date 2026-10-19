// Behaviour test for CHandleBasedMemoryPool::InternalAllocate (0x00A030D0): size rounding, the failure path, the
// free-list unlink, taken status, accounting, the largest-area refresh and the handle wiring.
#include <cstdio>
#include <string.h>
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
    virtual void v00() {}
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
    CHeader* m_free; long m_asked; int m_updates;
    CHeader* FindFreeArea(long size) { m_asked = size; return m_free; }
    void ResizeArea(CHeader* area, long newSize) { area->Padding = (short)(area->Size - newSize); area->Size = newSize; }
    void UpdateLargestFreeArea() { ++m_updates; }
    long InternalAllocateHandle() { return 2; }
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

int main()
{
    static CHandleBasedMemoryPool pool;
    static CHandleInfo slots[4]; pool.Handles = slots;
    pool.m_updates = 0; pool.NumAllocations = 0; pool.AllocatedSize = 0; pool.LargestFreeArea = 128;
    long handle = -7;
    pool.m_free = 0;
    int bad = 0;
    if (pool.InternalAllocate(handle, 33) || pool.m_asked != 48 || handle != -7) bad = 1;
    CHeader area; memset(&area, 0, sizeof(area));
    CHeader* bucket = &area; area.RefAvailable = &bucket; area.Status = 'N'; area.Size = 128;
    pool.m_free = &area;
    if (!pool.InternalAllocate(handle, 100) || pool.m_asked != 112) bad = 2;
    if (bucket != 0 || area.RefAvailable || area.Status != 'T' || area.Size != 112 || area.Padding != 16) bad = 3;
    if (pool.NumAllocations != 1 || pool.AllocatedSize != 112 + 16 + 32 || pool.m_updates != 1) bad = 4;
    if (handle != 2 || slots[2].Header != &area || area.Handle != 2) bad = 5;
    area.Size = 64; area.Status = 'N'; pool.LargestFreeArea = 999;
    pool.InternalAllocate(handle, 64);
    if (pool.m_updates != 1 || pool.NumAllocations != 2) bad = 6;
    if (bad) { std::printf("00a030d0_TEST FAIL\n"); return 1; }
    std::printf("00a030d0_TEST PASS\n");
    return 0;
}