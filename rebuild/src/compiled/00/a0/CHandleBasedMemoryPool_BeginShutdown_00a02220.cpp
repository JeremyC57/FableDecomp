// CHandleBasedMemoryPool::BeginShutdown(float timeout) -- under the pool lock: keep asking the failure handler to free the outstanding allocations, sleeping 10 ms with the lock released between rounds, until nothing is allocated (false once `timeout` seconds have passed); then hand the buffer back to the global allocator, clear the free lists, drop the buffer pointers and mark the pool shut down
// Leaf: BeginShutdown
// Flags: /d2inlT114
// Inline-budget: /d2inlT114 (EXTRA_RULES 2026-11-08 preference 2; replaces the nf140 FABLE_FORCEINLINE in the header) -- retail
//   expands the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, plain inline) here; at the default threshold both stay calls (DIFFER 305v334).
//   Sweep NF145: T74..T113 all DIFFER, T114 is the smallest match (T114..T2000 match on 0x00BD9B90; /d2inlS85 also matches there).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02220, length 332 bytes, `ret 4`
//   (Ego_r.pdb `?BeginShutdown@CHandleBasedMemoryPool@@QAE_NM@Z`).
// Callees: KERNEL32 EnterCriticalSection (IAT 0x0143FE0C, hoisted into ebp) / LeaveCriticalSection (IAT 0x0143FE10) /
//   Sleep (IAT 0x0143FE1C, hoisted into edi), GFGetTime @ 0x009E1BC0 (double), the failure handler's virtual slot 1, and
//   the inlined GFFreeFromGlobalAllocator body (landed @ 0x009A4350): CMemoryAllocator::EnterSynchronisation @ 0x009F2000 /
//   CMemoryAllocator::LeaveSynchronisation @ 0x009F2010, CMemoryAllocator::InternalDeallocate @ 0x009F2920 / GetAllocationCount
//   @ 0x009F1F10 / ~CMemoryAllocator @ 0x009F2310 / `__fastcall` operator delete @ 0x009F1F90 on the global allocator
//   @ 0x013CAA90 with its destroy-when-empty flag @ 0x013CAA94.
// Layout: Ego_r.pdb CHandleBasedMemoryPool names with the critical section at retail +0x44C (engine header +0x450: the
//   retail CArray<CHandleInfo> is 12 bytes), modelled locally as in the landed BeginCritical 0x00A02140 / Allocate.
// Shape: BeginCritical(false) / EndCritical inline; the drain loop is `while (AllocatedSize > 0) { if (!handler) break;
//   if (!handler->HandleFailure(..)) break; }` (rotated; its exit flags feed the `AllocatedSize == 0` test); the timeout is
//   `!(GFGetTime() <= start + timeout)` (`fcompp ; test ah,1 ; je`); GFFreeFromGlobalAllocator needs `__forceinline`; the
//   pointer/flag stores precede the memset of the free lists (`rep stosd`).
#include "fable_compiler.h"
#include <string.h>
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
// NF140 (q4197 / q4198): the frees are the inline public static CMemoryAllocator::Deallocate(block) (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z,
// out of line at 0x009A4350; engine/CMemoryAllocator.h opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, the body of the landed 0x009F2B20).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) int  __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FD30
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE10
extern "C" __declspec(dllimport) void __stdcall Sleep(unsigned long milliseconds);                         // IAT 0x0143FE1C
#include "rebuild_abi.h"
#include "engine/GFGetTime.h"                          // 0x009E1BC0 ?GFGetTime@@YINXZ (Ego_r.pdb / FableWin.pdb; NF133 q4294)
class CFailedAllocationHandler { public:
    virtual void v00();
    virtual bool HandleFailure(unsigned long size);    // slot 1 (+0x04)
};
struct CHeader;
class CHandleBasedMemoryPool { public:
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
    CMemoryAllocator::Deallocate(UnalignedBuffer);
    UnalignedBuffer = 0;
    Buffer = 0;
    ShutdownFlag = true;
    ClearFreeLists();
    EndCritical();
    return true;
}