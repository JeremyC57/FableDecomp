// CHandleBasedMemoryPool::Allocate(long* handle, unsigned long size, unsigned long flags) -- under the pool's scoped lock (try-lock when flags bit 0): refuse while shutting down; unless flags bit 1 or no failure handler, ask the handler to free memory until the pool has room, then (while the largest free area is too small) keep asking it up to the defrag threshold, try a partial defragment, ask again up to the failure threshold and try once more; finally allocate
// Leaf: Allocate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A03180, length 356 bytes, `ret 0Ch`
//   (Ego_r.pdb `?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z`).
// Callees: KERNEL32 TryEnterCriticalSection (IAT 0x0143FD30) / EnterCriticalSection (IAT 0x0143FE0C) /
//   LeaveCriticalSection (IAT 0x0143FE10), CScopedCritical::~CScopedCritical @ 0x00A03530 (out of line at the merged
//   failure exit), PartialDefragment(long) @ 0x00A02DF0, InternalAllocate(long*, unsigned long) @ 0x00A030D0, the failure
//   handler's virtual slot 1 (free memory for a request).
// Layout: Ego_r.pdb CHandleBasedMemoryPool names (PoolSize +0x0C, AllocatedSize +0x10, LargestFreeArea +0x1C,
//   MultiThreaded +0x24, ShutdownFlag +0x25, MaxFreeSizeBeforeDefrag +0x28, MaxFreeSizeBeforeFailure +0x2C,
//   FailureHandler +0x34); the critical section sits at +0x44C in retail (engine/CHandleBasedMemoryPool.h has +0x450: the
//   retail CArray<CHandleInfo> is 12 bytes), so `this` is modelled locally as in the landed BeginCritical 0x00A02140.
// Shape: the lock is a scoped {pool, locked} object whose constructor inlines BeginCritical and whose destructor inlines
//   EndCritical except at the merged failure exit; the two handler waits are `while (largest < size) { if (!a) break;
//   if (!b) break; }` (rotated; an empty-bodied `while (a && b && c);` stays top-tested); the "no handler" flag test is
//   `(unsigned char)~(flags >> 1) & 1` (`shr ebx,1 ; not bl ; test bl,1`).
struct _RTL_CRITICAL_SECTION_Retail { long m_words[6]; };   // 24 bytes
extern "C" __declspec(dllimport) int  __stdcall TryEnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);   // IAT 0x0143FD30
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(_RTL_CRITICAL_SECTION_Retail* cs);      // IAT 0x0143FE10
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
    bool PartialDefragment(long size);                       // 0x00A02DF0
    bool InternalAllocate(long* handle, unsigned long size); // 0x00A030D0
    bool Allocate(long* handle, unsigned long size, unsigned long flags);
};
struct CScopedCritical {
    CHandleBasedMemoryPool* m_pPool;
    bool                    m_locked;
    CScopedCritical(CHandleBasedMemoryPool* pool, bool tryOnly) : m_pPool(pool) { m_locked = pool->BeginCritical(tryOnly); }
    ~CScopedCritical()                                       // 0x00A03530
    {
        if (m_locked)
            m_pPool->EndCritical();
    }
};

bool CHandleBasedMemoryPool::Allocate(long* handle, unsigned long size, unsigned long flags)
{
    CScopedCritical lock(this, (flags & 1) != 0);
    if (!lock.m_locked)
        return false;
    if (ShutdownFlag)
        return false;
    if (FailureHandler && ((unsigned char)~(flags >> 1) & 1)) {
        while (size > (unsigned long)(PoolSize - AllocatedSize)) {
            if (!FailureHandler->HandleFailure(size))
                return false;
        }
        while ((unsigned long)LargestFreeArea < size) {
            if ((unsigned long)(PoolSize - AllocatedSize) >= MaxFreeSizeBeforeDefrag + size)
                break;
            if (!FailureHandler->HandleFailure(size))
                break;
        }
        if ((unsigned long)LargestFreeArea < size && !PartialDefragment(size)) {
            while ((unsigned long)LargestFreeArea < size) {
                if ((unsigned long)(PoolSize - AllocatedSize) >= MaxFreeSizeBeforeFailure + size)
                    break;
                if (!FailureHandler->HandleFailure(size))
                    break;
            }
            if ((unsigned long)LargestFreeArea < size && !PartialDefragment(size))
                return false;
        }
    }
    bool result = InternalAllocate(handle, size);
    return result;
}