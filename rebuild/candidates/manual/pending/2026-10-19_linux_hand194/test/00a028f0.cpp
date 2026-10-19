// Behaviour test for CHandleBasedMemoryPool::MoveArea (0x00A028F0): the move callback arguments, dest unlinked from its
// free list and trimmed, header fields and handle slot moved, padding difference accounted, source freed.
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

struct CHandleBasedMemoryPool {
    virtual void v00() {}
    void* m_res; long m_handle; void* m_from; void* m_to; long m_size; int m_inserted; CHeader* m_resized; long m_newSize;
    virtual void MoveResourceData(void* resource, long handle, void* from, void* to, long size)
    { m_res = resource; m_handle = handle; m_from = from; m_to = to; m_size = size; }
    CHeader*      Buffer;                          // +0x04
    void*         UnalignedBuffer;                 // +0x08
    long          PoolSize;                        // +0x0c
    long          AllocatedSize;                   // +0x10
    unsigned char _pad14[0x38 - 0x14];
    CHeader*      FreeLists[256];                  // +0x38
    CHeader**     Handles;                         // +0x438 CArray<CHandleInfo>: first word
    void ResizeArea(CHeader* area, long newSize) { m_resized = area; m_newSize = newSize; area->Size = newSize; area->Padding = 4; }
    void InsertFreeArea(CHeader* area) { ++m_inserted; area->RefAvailable = &FreeLists[0]; FreeLists[0] = area; }
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

int main()
{
    static CHandleBasedMemoryPool pool; memset(&pool.AllocatedSize, 0, sizeof(long));
    pool.m_inserted = 0; pool.AllocatedSize = 1000;
    static CHeader* slots[8]; pool.Handles = slots;
    CHeader src, dst, after; memset(&src, 0, sizeof(src)); memset(&dst, 0, sizeof(dst)); memset(&after, 0, sizeof(after));
    int resource = 0;
    src.Resource = &resource; src.Handle = 5; src.Size = 64; src.Padding = 12; src.Status = 'T'; src.Locked = 1;
    CHeader* bucket = &dst;                        // dst heads a free list, followed by `after`
    dst.Status = 'N'; dst.Size = 256; dst.RefAvailable = &bucket; dst.NextAvailable = &after; after.RefAvailable = &dst.NextAvailable;
    pool.MoveArea(&src, &dst);
    int bad = 0;
    if (pool.m_res != &resource || pool.m_handle != 5 || pool.m_from != (char*)&src + 0x20 || pool.m_to != (char*)&dst + 0x20 || pool.m_size != 64) bad = 1;
    if (bucket != &after || after.RefAvailable != &bucket || dst.RefAvailable != 0 || dst.NextAvailable != 0) bad = 2;
    if (pool.m_resized != &dst || pool.m_newSize != 64) bad = 3;
    if (dst.Resource != &resource || dst.Handle != 5 || dst.Status != 'T' || dst.Locked != 1 || slots[5] != &dst) bad = 4;
    if (pool.AllocatedSize != 1000 + 4 - 12) bad = 5;
    if (src.Status != 'N' || pool.m_inserted != 1 || pool.FreeLists[0] != &src) bad = 6;
    if (bad) { std::printf("00a028f0_TEST FAIL\n"); return 1; }
    std::printf("00a028f0_TEST PASS\n");
    return 0;
}
