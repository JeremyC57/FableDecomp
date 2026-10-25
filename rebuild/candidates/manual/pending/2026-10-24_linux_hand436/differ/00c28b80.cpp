// CHandleBasedMemoryPool::InternalAllocate(long* handle, long size) -- round the request up to 16 bytes, take the best-fit free area (FindFreeArea), unlink it, mark it taken ('T'), split off the tail (ResizeArea), count the allocation and its bytes (size + padding + the 0x20-byte header), refresh the largest-free-area word when the taken area was the largest, then give it a handle (InternalAllocateHandle): store the handle, point the handle slot at the area and the area back at its handle; 1 on success, 0 when no free area fits
// Grade: DIFFER(145v147) -- the handle-slot store: retail `lea eax,[edx+eax*4] ; mov [eax],edi`, ours `mov [edx+eax*4],edi` (everything else matches)
// Leaf: InternalAllocate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28B80, length 160 bytes (147 + int3 padding), `ret 8`.
// Callees: FindFreeArea(long) @ 0x00C27FD0, RemoveFreeArea(CHeader*) @ 0x00C27F50, ResizeArea(CHeader*, long) @ 0x00C28290
//   (catalog "Thiscall_BlockAllocationShrink"), UpdateLargestFreeArea() @ 0x00C28080, InternalAllocateHandle() @ 0x00C28A80
//   (all landed or named from their bodies). The only caller is Allocate @ 0x00C28C20 (the tail call at 0x00C28CEA).
// Name: the second (0x00C27xxx-0x00C28xxx) build of the pool; its 0x00A0xxxx twin is Ego_r.pdb
//   `?InternalAllocate@CHandleBasedMemoryPool@@AAE_NPAJK@Z` @ 0x00A030D0 (the same body with a bool result and an inline
//   RemoveFreeArea). This build returns a BOOL in eax (`mov eax,1` / `xor eax,eax`).
// Layout: local retail view of this build, as in the landed ctor 0x00C28AE0 and FreeHandle 0x00C28610 (engine/
//   CHandleBasedMemoryPool.h is the Ego_r layout: FreeLists +0x38, Handles +0x438; this build has an embedded default
//   failure handler at +0x34, FailureHandler +0x38, FreeLists +0x3c and the handle table pointer at +0x440).
// Shape: the round-up is an unsigned `>> 4 << 4` (`shr ebp,4 ; shl ebp,4`); the was-largest flag is an int compare
//   (`sub ; neg ; sbb ; inc`, tested as a dword) taken before RemoveFreeArea; the handle slot is written through a
//   reference (`lea eax,[edx+eax*4] ; mov [eax],edi`) and the area's handle word re-reads *handle.
struct CHeader {                            // CHandleBasedMemoryPool::CHeader (32 bytes)
    void*          Resource;                // +0x00
    long           Handle;                  // +0x04
    long           Size;                    // +0x08
    short          Padding;                 // +0x0c
    unsigned char  Status;                  // +0x0e ('T' = taken, 'N' = free)
    unsigned char  Locked;                  // +0x0f
    CHeader**      RefAvailable;            // +0x10
    CHeader*       NextAvailable;           // +0x14
    CHeader*       PrevContiguous;          // +0x18
    CHeader*       NextContiguous;          // +0x1c
};
union CHandleSlot {                         // a handle table entry
    CHeader* Header;                        // a live handle's block header
    long     NextFreeHandle;                // a free handle's link in the free-handle chain
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
    unsigned char  _pad_0x20[0x420];        // +0x20 thresholds, failure handlers, FreeLists (+0x3c)
    CHandleSlot*   Handles;                 // +0x440 handle table data pointer
    unsigned char  _pad_0x444[0x8];
    long           FirstUnusedHandle;       // +0x44c

    CHeader* FindFreeArea(long size);               // 0x00C27FD0
    void     RemoveFreeArea(CHeader* area);         // 0x00C27F50
    void     ResizeArea(CHeader* area, long size);  // 0x00C28290
    void     UpdateLargestFreeArea();               // 0x00C28080
    long     InternalAllocateHandle();              // 0x00C28A80
    int      InternalAllocate(long* handle, long size);
};

int CHandleBasedMemoryPool::InternalAllocate(long* handle, long size)
{
    size = (long)(((unsigned long)size + 15) / 16 * 16);
    CHeader* area = FindFreeArea(size);
    if (area) {
        int wasLargest = (area->Size == LargestFreeArea);
        RemoveFreeArea(area);
        area->Status = 'T';
        ResizeArea(area, size);
        ++NumAllocations;
        AllocatedSize += area->Padding + area->Size + (long)sizeof(CHeader);
        if (wasLargest)
            UpdateLargestFreeArea();
        long index = InternalAllocateHandle();
        *handle = index;
        CHandleSlot& slot = Handles[index];
        slot.Header = area;
        area->Handle = *handle;
        return 1;
    }
    return 0;
}
