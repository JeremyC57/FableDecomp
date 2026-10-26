// CHandleBasedMemoryPool::MoveArea(CHeader* source, CHeader* dest) -- defragmentation step: copy the resource's bytes into the free area `dest` (virtual slot 1), take `dest` off its free list, trim it to the source size, move the header fields and the handle table entry across, adjust the allocated-size padding difference and free `source`
// Leaf: MoveArea
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A028F0, 163 bytes, `ret 8`.
// Callees: this pool's virtual slot 1 (+4: move the resource bytes; arguments resource, handle, from, to, size),
//   CHandleBasedMemoryPool::ResizeArea @ 0x00A02410 and CHandleBasedMemoryPool::InsertFreeArea @ 0x00A01DA0 (both landed).
// Layout: Ego_r.pdb CHandleBasedMemoryPool names (AllocatedSize +0x10, FreeLists +0x38, Handles +0x438: the first word of
//   retail's 12-byte CArray is the slot array); engine/CHandleBasedMemoryPool.h flattens the vptr into a `void*`, so the
//   virtual slot needs the local polymorphic model (as in the landed ValidateResources 0x00A02AD0). CHeader is the
//   Ego_r.pdb 32-byte record (Resource, Handle, Size, Padding, Status, Locked, RefAvailable, NextAvailable,
//   PrevContiguous, NextContiguous).
// Shape: the free-list unlink of `dest` is an inline member (`if (RefAvailable && Status == 'N')`, one zero register for
//   the status and both link clears); the padding adjustment is `AllocatedSize += dest->Padding - source->Padding`.
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

class CHandleBasedMemoryPool { public:
    virtual void v00();
    virtual void MoveResourceData(void* resource, long handle, void* from, void* to, long size);   // slot 1 (+4)
    CHeader*      Buffer;                          // +0x04
    void*         UnalignedBuffer;                 // +0x08
    long          PoolSize;                        // +0x0c
    long          AllocatedSize;                   // +0x10
    unsigned char _pad14[0x38 - 0x14];
    CHeader*      FreeLists[256];                  // +0x38
    CHeader**     Handles;                         // +0x438 CArray<CHandleInfo>: first word
    void ResizeArea(CHeader* area, long newSize);  // 0x00A02410
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void MoveArea(CHeader* source, CHeader* dest);
};

void CHandleBasedMemoryPool::MoveArea(CHeader* source, CHeader* dest)
{
    long size = source->Size;
    MoveResourceData(source->Resource, source->Handle, (char*)source + 0x20, (char*)dest + 0x20, size);
    dest->RemoveFromFreeList();
    ResizeArea(dest, size);
    dest->Resource = source->Resource;
    dest->Status = source->Status;
    dest->Handle = source->Handle;
    dest->Locked = source->Locked;
    Handles[source->Handle] = dest;
    AllocatedSize += dest->Padding - source->Padding;
    source->Status = 'N';
    InsertFreeArea(source);
}