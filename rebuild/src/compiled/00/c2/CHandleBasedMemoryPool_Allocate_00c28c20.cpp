// CHandleBasedMemoryPool::Allocate(long* handle, long size) -- when a failure handler is installed: ask it to free memory until the pool has `size` bytes left (0 when it gives up); while the largest free area is too small keep asking it until the free total reaches the defrag threshold (+0x20) above `size`, then try a partial defragment; failing that ask again up to the failure threshold (+0x24), try a second partial defragment and, when the full-defrag flag (+0x28) is set, defragment the whole pool (0 when it is clear); finally allocate (InternalAllocate)
// Leaf: Allocate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28C20, length 219 bytes, `ret 8`.
// Callees: the failure handler's virtual slot 1 (free memory for a request, BOOL), PartialDefragment(long) @ 0x00C28870,
//   Defragment() @ 0x00C28400, InternalAllocate(long*, long) @ 0x00C28B80. The only caller is 0x00C22152.
// Name: the second (0x00C27xxx-0x00C28xxx) build of the pool; its 0x00A0xxxx twin is Ego_r.pdb
//   `?Allocate@CHandleBasedMemoryPool@@QAE_NPAJKK@Z` @ 0x00A03180 (landed). This build has no scoped lock, no flags word
//   and no shutdown test, compares signed, and adds a full Defragment() fallback behind the +0x28 flag.
// Layout: local retail view of this build, as in the landed ctor 0x00C28AE0 (which stores the defaults +0x20 = 0x80000,
//   +0x24 = 0x100000, +0x28 = 1, +0x2c = 0x10 and FailureHandler +0x38 = &the embedded default handler at +0x34).
// Shape: the handler waits are written as in the landed 0x00A03180 (`while (largest < size) { if (a) break; if (!b) break; }`,
//   rotated with the tests duplicated at the loop exits); the result of InternalAllocate is returned directly.
struct CHeader;
struct CFailedAllocationHandler {
    virtual void v00();
    virtual int  HandleFailure(long size);  // slot 1 (+0x04)
};
struct CHandleBasedMemoryPool {             // retail view of the 0x00C27xxx build (vtable 0x012B4098)
    void*          __vftable;               // +0x00
    void*          Buffer;                  // +0x04
    void*          UnalignedBuffer;         // +0x08
    long           PoolSize;                // +0x0c
    long           AllocatedSize;           // +0x10
    long           NumAllocations;          // +0x14
    long           LockCount;               // +0x18
    long           LargestFreeArea;         // +0x1c
    long           MaxFreeSizeBeforeDefrag; // +0x20 (0x80000)
    long           MaxFreeSizeBeforeFailure; // +0x24 (0x100000)
    long           EnableFullDefragIfPartialFails; // +0x28 (1)
    long           MaxBlocksToTryOnPartialDefrag;  // +0x2c (0x10)
    long           m_30;                    // +0x30
    void*          DefaultFailureHandler;   // +0x34 (embedded handler's vptr)
    CFailedAllocationHandler* FailureHandler; // +0x38

    int  PartialDefragment(long size);              // 0x00C28870
    void Defragment();                              // 0x00C28400
    int  InternalAllocate(long* handle, long size); // 0x00C28B80
    int  Allocate(long* handle, long size);
};

int CHandleBasedMemoryPool::Allocate(long* handle, long size)
{
    if (FailureHandler) {
        while (size > PoolSize - AllocatedSize) {
            if (!FailureHandler->HandleFailure(size))
                return 0;
        }
        while (LargestFreeArea < size) {
            if (PoolSize - AllocatedSize >= MaxFreeSizeBeforeDefrag + size)
                break;
            if (!FailureHandler->HandleFailure(size))
                break;
        }
        if (LargestFreeArea < size && !PartialDefragment(size)) {
            while (LargestFreeArea < size) {
                if (PoolSize - AllocatedSize >= MaxFreeSizeBeforeFailure + size)
                    break;
                if (!FailureHandler->HandleFailure(size))
                    break;
            }
            if (LargestFreeArea < size && !PartialDefragment(size)) {
                if (EnableFullDefragIfPartialFails)
                    Defragment();
                else
                    return 0;
            }
        }
    }
    return InternalAllocate(handle, size);
}