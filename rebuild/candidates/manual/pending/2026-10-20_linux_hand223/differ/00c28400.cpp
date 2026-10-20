// Grade: DIFFER(285v310) -- register allocation (retail: offset ebx / next ebp; ours: offset ebp / next edx), frame-slot order, and retail's unfolded handle-table addressing (lea r,[h*4]; add; store, plus a dead add after the second store)
// CHandleBasedMemoryPool::Defragment() -- unless the pool is locked: slide every used area down to the front of the buffer (moving each resource through the virtual MoveResource and re-pointing its handle), rebuild the contiguous chain, then turn the tail into one free area
// Leaf: Defragment
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28400, length 310 bytes, plain `ret`.
// Signature from Ego_r.pdb: ?Defragment@CHandleBasedMemoryPool@@QAEXXZ.
// Callees: the virtual MoveResource(void*, long, void*, void*, unsigned long) (slot 0 of this copy's vtable),
//   ClearFreeLists @ 0x00C28200, CreateArea(void*, unsigned long, CHeader*, CHeader*) @ 0x00C27EA0, InsertFreeArea(CHeader*)
//   @ 0x00C27EF0 (catalog labels "$E1" / "WordWrap_SetCallback" are propagated).
// Layout: engine/CHandleBasedMemoryPool.h up to +0x38 (Buffer +4, PoolSize +0xc, AllocatedSize +0x10, LockCount +0x18);
//   the handle table's element pointer is read at +0x440 (retail; the header's 16-byte PDB CArray at +0x438), so `this`
//   is a local model. CHeader (PDB, 32 bytes): Resource +0, Handle +4, Size +8, Padding +0xc, Status +0xe, Locked +0xf,
//   RefAvailable +0x10, NextAvailable +0x14, PrevContiguous +0x18, NextContiguous +0x1c.
// Shape: the running offset is kept in ebx and in a frame word; a used area (no free-list back pointer) that is not
//   already in place (or is padded) is moved and its header rebuilt field by field ('T' status) and linked after the
//   previous one; an area already in place only advances the offset.
struct CHandleBasedMemoryPool {
    struct CHeader {
        void*          Resource;                        // +0x00
        long           Handle;                          // +0x04
        unsigned long  Size;                            // +0x08
        short          Padding;                         // +0x0c
        unsigned char  Status;                          // +0x0e
        unsigned char  Locked;                          // +0x0f
        CHeader**      RefAvailable;                    // +0x10
        CHeader*       NextAvailable;                   // +0x14
        CHeader*       PrevContiguous;                  // +0x18
        CHeader*       NextContiguous;                  // +0x1c
    };
    virtual void MoveResource(void* resource, long handle, void* from, void* to, unsigned long size);   // slot 0
    void*         Buffer;                               // +0x04
    void*         UnalignedBuffer;                      // +0x08
    long          PoolSize;                             // +0x0c
    long          AllocatedSize;                        // +0x10
    long          NumAllocations;                       // +0x14
    long          LockCount;                            // +0x18
    unsigned char _pad1c[0x440 - 0x1c];
    CHeader**     HandleTable;                          // +0x440 (the handle array's elements: one header pointer each)

    void ClearFreeLists();                              // 0x00C28200
    CHeader* CreateArea(void* at, unsigned long size, CHeader* prev, CHeader* next);   // 0x00C27EA0
    void InsertFreeArea(CHeader* area);                 // 0x00C27EF0
    void Defragment();
};

void CHandleBasedMemoryPool::Defragment()
{
    if (LockCount > 0)
        return;
    CHeader* prev = 0;
    long offset = 0;
    CHeader* header = (CHeader*)Buffer;
    while (header) {
        CHeader* next = header->NextContiguous;
        if (!header->RefAvailable) {
            CHeader* dest = (CHeader*)((char*)Buffer + offset);
            unsigned long size = header->Size;
            unsigned char locked = header->Locked;
            if (header != dest || header->Padding != 0) {
                long handle = header->Handle;
                void* resource = header->Resource;
                HandleTable[handle] = 0;
                MoveResource(resource, handle, header + 1, dest + 1, size);
                HandleTable[handle] = dest;
                dest->Locked = locked;
                dest->Resource = resource;
                dest->Handle = handle;
                dest->Size = size;
                dest->Padding = 0;
                dest->Status = 'T';
                dest->RefAvailable = 0;
                dest->NextAvailable = 0;
                dest->PrevContiguous = prev;
                dest->NextContiguous = 0;
                if (prev)
                    prev->NextContiguous = dest;
                prev = dest;
            }
            offset += size + sizeof(CHeader);
        }
        header = next;
    }
    AllocatedSize = offset;
    ClearFreeLists();
    long freeSize = PoolSize - offset;
    if (freeSize > (long)sizeof(CHeader))
        InsertFreeArea(CreateArea((char*)Buffer + offset, freeSize, prev, 0));
}
