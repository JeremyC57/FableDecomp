// CHandleBasedMemoryPool::Validate() -- debug consistency walk of the pool under its lock: find the highest non-empty free-list bucket and its largest area, then for every area (physical chain) walk all 256 free lists; the checks themselves are compiled out of the shipped build, only the walks remain
// Leaf: Validate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A024C0, 161 bytes.
// Callees: EnterCriticalSection / LeaveCriticalSection through their imports (0x0143FE0C / 0x0143FE10), only when
//   MultiThreaded.
// Layout: Ego_r.pdb CHandleBasedMemoryPool names; engine/CHandleBasedMemoryPool.h gives Handles a 16-byte CArray, retail's
//   is 12 bytes (landed ValidateResources 0x00A02AD0), so everything after +0x438 sits 4 bytes lower in retail
//   (CriticalSection +0x44c, not the header's +0x450) and `this` is modelled locally: Buffer +4, MultiThreaded +0x24,
//   FreeLists[256] +0x38, Handles +0x438, FirstUnusedHandle +0x444, Name +0x448, CriticalSection +0x44c.
//   CHeader (32 bytes, Ego_r.pdb): Size +8, NextAvailable +0x14, NextContiguous +0x1c.
// Shape: the bucket search counts down from 255 with the pointer stepped alongside (`sub ecx,4`); the hit block (inside
//   the loop, ending in `break`) re-reads FreeLists[index]; the largest size is computed into a dead register (its ASSERT is compiled out); the outer walk
//   reads NextContiguous before the inner 256-bucket walk.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);   // 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);   // 0x0143FE10

struct CHeader {                                   // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    void*     Resource;                            // +0x00
    long      Handle;                              // +0x04
    long      Size;                                // +0x08
    short     Padding;                             // +0x0c
    unsigned char Status;                          // +0x0e
    unsigned char Locked;                          // +0x0f
    CHeader** RefAvailable;                        // +0x10
    CHeader*  NextAvailable;                       // +0x14
    CHeader*  PrevContiguous;                      // +0x18
    CHeader*  NextContiguous;                      // +0x1c
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
    CHeader*      Buffer;                          // +0x04 (the first area of the physical chain)
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
    void*         Handles[3];                      // +0x438 CArray<CHandleInfo> (12 bytes in retail)
    long          FirstUnusedHandle;               // +0x444
    void*         Name;                            // +0x448 CCharString
    SCriticalSection CriticalSection;              // +0x44c
    void Validate();
};

void CHandleBasedMemoryPool::Validate()
{
    if (MultiThreaded)
        EnterCriticalSection(&CriticalSection);
    for (long index = 255; index >= 0; --index) {
        if (FreeLists[index] != 0) {
            long largest = 0;
            for (CHeader* area = FreeLists[index]; area; area = area->NextAvailable)
                if (largest < area->Size)
                    largest = area->Size;
            break;
        }
    }
    CHeader* next;
    for (CHeader* area = Buffer; area; area = next) {
        next = area->NextContiguous;
        for (long list = 0; list < 256; ++list)
            for (CHeader* free = FreeLists[list]; free; free = free->NextAvailable)
                ;
    }
    if (MultiThreaded)
        LeaveCriticalSection(&CriticalSection);
}
