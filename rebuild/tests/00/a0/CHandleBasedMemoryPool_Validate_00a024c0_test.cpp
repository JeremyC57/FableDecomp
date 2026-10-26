// Behaviour test for CHandleBasedMemoryPool::Validate (0x00A024C0): the walks terminate over a populated pool and the
// critical section is entered/left exactly when the pool is multi-threaded.
#include <cstdio>
#include <string.h>
static int g_enter = 0, g_leave = 0; static void* g_section = 0;
void EnterCriticalSection_(void* s) { ++g_enter; g_section = s; }
void LeaveCriticalSection_(void* s) { if (s == g_section) ++g_leave; }
#define EnterCriticalSection EnterCriticalSection_
#define LeaveCriticalSection LeaveCriticalSection_
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

class CHandleBasedMemoryPool { public:
    virtual void v00() {}
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

int main()
{
    static CHandleBasedMemoryPool pool;
    memset(pool.FreeLists, 0, sizeof(pool.FreeLists));
    CHeader a[4]; memset(a, 0, sizeof(a));
    for (int i = 0; i < 3; ++i) a[i].NextContiguous = &a[i + 1];
    a[1].Size = 100; a[3].Size = 300; a[1].NextAvailable = &a[3];
    pool.FreeLists[7] = &a[1]; pool.FreeLists[200] = &a[3];
    pool.Buffer = &a[0];
    pool.MultiThreaded = true;
    pool.Validate();
    int bad = 0;
    if (g_enter != 1 || g_leave != 1 || g_section != &pool.CriticalSection) bad = 1;
    pool.MultiThreaded = false; pool.Buffer = 0; memset(pool.FreeLists, 0, sizeof(pool.FreeLists));
    pool.Validate();
    if (g_enter != 1 || g_leave != 1) bad = 2;
    if (bad) { std::printf("00a024c0_TEST FAIL\n"); return 1; }
    std::printf("00a024c0_TEST PASS\n");
    return 0;
}