// CHandleBasedMemoryPool::ResizeArea(CHeader* area, long newSize) -- shrink an area: when more than a header's worth (32 bytes) is spare, carve a new free area after `newSize` bytes, link it physically and insert it in the free lists; otherwise keep the spare bytes as the area's padding
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02410, length 96 bytes.
// Callees: CHandleBasedMemoryPool::InsertFreeArea @ 0x00A01DA0 (on `this`). Shape: the spare computed unsigned (`cmp edx,20h ; jbe`), the new header at `area + newSize + 32`
// (`lea eax,[esi+ebx+20h]`), its words stored with a zero register (the type byte 'N' as an immediate), the physical successor kept in edi and re-linked after the test, `ret 8`.
// Model: engine/CHandleBasedMemoryPool.h (LargestFreeArea +0x1c, FreeLists[256] +0x38 agree with the bytes); CHeader is reconstructed from the free-list bodies (no PDB layout).
struct CHeader {                                  // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    long           Handle;                         // +0x00
    long           _unused04;                      // +0x04
    long           Size;                           // +0x08
    short          Padding;                        // +0x0c (bytes folded back into Size when the area is freed)
    char           Type;                           // +0x0e ('N' free, 'T' taken)
    char           Flags;                          // +0x0f
    CHeader**      PPrevFreeLink;                  // +0x10 (the link that points at this free area)
    CHeader*       PNextFree;                      // +0x14 (the free-list chain)
    CHeader*       PPrevPhysical;                  // +0x18
    CHeader*       PNextPhysical;                  // +0x1c
};
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void ResizeArea(CHeader* area, long newSize);
};

void CHandleBasedMemoryPool_Methods::ResizeArea(CHeader* area, long newSize)
{
    unsigned long spare = area->Size - newSize;
    if (spare > 0x20) {
        CHeader* fresh = (CHeader*)((char*)area + newSize + 0x20);
        CHeader* next = area->PNextPhysical;
        fresh->Size = spare - 0x20;
        fresh->Padding = 0;
        fresh->Type = 'N';
        fresh->Flags = 0;
        fresh->PPrevPhysical = area;
        fresh->PNextPhysical = next;
        fresh->Handle = 0;
        area->PNextPhysical = fresh;
        if (next)
            next->PPrevPhysical = fresh;
        InsertFreeArea(fresh);
        area->Size = newSize;
    } else {
        area->Size = newSize;
        area->Padding = (short)spare;
    }
}
