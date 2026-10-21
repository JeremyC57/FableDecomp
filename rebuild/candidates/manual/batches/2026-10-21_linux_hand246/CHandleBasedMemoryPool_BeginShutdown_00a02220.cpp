// CHandleBasedMemoryPool::BeginShutdown(float timeout) -- under the pool lock: keep asking the failure handler to free the outstanding allocations, sleeping 10 ms with the lock released between rounds, until nothing is allocated (false once `timeout` seconds have passed); then hand the buffer back to the global allocator, clear the free lists, drop the buffer pointers and mark the pool shut down
// Leaf: BeginShutdown
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02220, length 332 bytes, `ret 4`
//   (Ego_r.pdb `?BeginShutdown@CHandleBasedMemoryPool@@QAE_NM@Z`).
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C, hoisted into ebp) / LeaveCriticalSection (IAT 0x0143FE10) /
//   Sleep (IAT 0x0143FE1C, hoisted into edi), GFGetTime @ 0x009E1BC0 (double), the failure handler's virtual slot 1, and
//   the inlined GFFreeFromGlobalAllocator body (landed @ 0x009A4350): GFEnterAllocatorLock @ 0x009F2000 /
//   GFLeaveAllocatorLock @ 0x009F2010, CMemoryAllocator::InternalDeallocate @ 0x009F2920 / GetAllocationCount
//   @ 0x009F1F10 / ~CMemoryAllocator @ 0x009F2310 / `__fastcall` operator delete @ 0x009F1F90 on the global allocator
//   @ 0x013CAA90 with its destroy-when-empty flag @ 0x013CAA94.
// Layout: Ego_r.pdb CHandleBasedMemoryPool names with the critical section at retail +0x44C (engine header +0x450: the
//   retail CArray<CHandleInfo> is 12 bytes), modelled locally as in the landed BeginCritical 0x00A02140 / Allocate.
// Shape: BeginCritical(false) / EndCritical inline; the drain loop is `while (AllocatedSize > 0) { if (!handler) break;
//   if (!handler->HandleFailure(..)) break; }` (rotated; its exit flags feed the `AllocatedSize == 0` test); the timeout is
//   `!(GFGetTime() <= start + timeout)` (`fcompp ; test ah,1 ; je`); GFFreeFromGlobalAllocator needs `__forceinline`; the
//   pointer/flag stores precede the memset of the free lists (`rep stosd`).
#include <string.h>
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) int  __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FD30
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);                         // IAT 0x0143FE1C
double GFGetTime();                                // 0x009E1BC0
struct CMemoryAllocator {
    void InternalDeallocate(void* block);          // 0x009F2920
    long GetAllocationCount();                     // 0x009F1F10
    ~CMemoryAllocator();                           // 0x009F2310
    static void __fastcall operator delete(void* block);   // 0x009F1F90
};
void __cdecl GFEnterAllocatorLock();               // 0x009F2000
void __cdecl GFLeaveAllocatorLock();               // 0x009F2010
extern CMemoryAllocator* g_pGlobalAllocator_013caa90;             // 0x013CAA90
extern bool              g_bDestroyAllocatorWhenEmpty_013caa94;   // 0x013CAA94
__forceinline void __fastcall GFFreeFromGlobalAllocator(void* block)    // 0x009A4350 (inlined here)
{
    GFEnterAllocatorLock();
    if (block && g_pGlobalAllocator_013caa90) {
        g_pGlobalAllocator_013caa90->InternalDeallocate(block);
        if (g_bDestroyAllocatorWhenEmpty_013caa94) {
            if (g_pGlobalAllocator_013caa90->GetAllocationCount() == 0) {
                delete g_pGlobalAllocator_013caa90;
                g_pGlobalAllocator_013caa90 = 0;
            }
        }
    }
    GFLeaveAllocatorLock();
}
struct CFailedAllocationHandler {
    virtual void v00();
    virtual bool HandleFailure(unsigned long size);    // slot 1 (+0x04)
};
struct CHeader;
struct CHandleBasedMemoryPool {
    virtual void v00();
    void*                        Buffer;                     // +0x04
    void*                        UnalignedBuffer;            // +0x08
    long                         PoolSize;                   // +0x0c
    long                         AllocatedSize;              // +0x10
    long                         NumAllocations;             // +0x14
    long                         LockCount;                  // +0x18
    long                         LargestFreeArea;            // +0x1c
    unsigned long                MemoryFlags;                // +0x20
    bool                         MultiThreaded;              // +0x24
    bool                         ShutdownFlag;               // +0x25
    bool                         EnableDefrag;               // +0x26
    bool                         EnableFullDefragIfPartialFails; // +0x27
    unsigned long                MaxFreeSizeBeforeDefrag;    // +0x28
    unsigned long                MaxFreeSizeBeforeFailure;   // +0x2c
    long                         MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler*    FailureHandler;             // +0x34
    CHeader*                     FreeLists[256];             // +0x38
    unsigned char                Handles[0x0c];              // +0x438 CArray<CHandleInfo> (retail 12 bytes)
    long                         FirstUnusedHandle;          // +0x444
    void*                        Name;                       // +0x448 CCharString
    _RTL_CRITICAL_SECTION_Retail CriticalSection;            // +0x44c

    bool BeginCritical(bool tryOnly)
    {
        if (!MultiThreaded)
            return true;
        if (tryOnly) {
            if (!TryEnterCriticalSection(&CriticalSection))
                return false;
        } else {
            EnterCriticalSection(&CriticalSection);
        }
        return true;
    }
    void EndCritical()
    {
        if (MultiThreaded)
            LeaveCriticalSection(&CriticalSection);
    }
    void ClearFreeLists() { memset(FreeLists, 0, sizeof(FreeLists)); }
    bool BeginShutdown(float timeout);
};

bool CHandleBasedMemoryPool::BeginShutdown(float timeout)
{
    BeginCritical(false);
    double startTime = GFGetTime();
    for (;;) {
        while (AllocatedSize > 0) {
            if (!FailureHandler)
                break;
            if (!FailureHandler->HandleFailure(AllocatedSize))
                break;
        }
        if (AllocatedSize == 0)
            break;
        EndCritical();
        Sleep(10);
        BeginCritical(false);
        if (!(GFGetTime() <= startTime + timeout)) {
            EndCritical();
            return false;
        }
    }
    GFFreeFromGlobalAllocator(UnalignedBuffer);
    UnalignedBuffer = 0;
    Buffer = 0;
    ShutdownFlag = true;
    ClearFreeLists();
    EndCritical();
    return true;
}
