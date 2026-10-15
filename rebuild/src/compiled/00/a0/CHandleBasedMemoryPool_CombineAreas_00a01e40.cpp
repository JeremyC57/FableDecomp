// CHandleBasedMemoryPool::CombineAreas(CHeader* first, CHeader* second) -- merge two physically adjacent areas: unlink both from their free lists, splice `second` out of the physical chain, grow `first` by second's size plus its 32-byte header, mark it free and re-insert it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01E40, length 141 bytes.
// Callees: CHandleBasedMemoryPool::InsertFreeArea(CHeader*) @ 0x00A01DA0.
// Shape: the free-list unlink is an inline helper applied to each area (null back-link and 'N' type test, type
// cleared before the link store, the successor's back link re-pointed, both links zeroed); 'N' lives in cl and
// zero in ebx for the whole body. The size grows by `second->Size + sizeof(CHeader)` (the 0x20 added to the
// second size first).
// Model: engine/CHandleBasedMemoryPool.h; CHeader as landed with InsertFreeArea (0x00A01DA0).
struct CHeader {                                  // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    long           Handle;                         // +0x00
    long           _unused04;                      // +0x04
    long           Size;                           // +0x08
    short          Padding;                        // +0x0c
    char           Type;                           // +0x0e ('N' free, 'T' taken)
    char           Flags;                          // +0x0f
    CHeader**      PPrevFreeLink;                  // +0x10 (the link that points at this free area)
    CHeader*       PNextFree;                      // +0x14
    CHeader*       PPrevPhysical;                  // +0x18
    CHeader*       PNextPhysical;                  // +0x1c
};
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void InsertFreeArea(CHeader* area);            // 0x00A01DA0
    void CombineAreas(CHeader* first, CHeader* second);
    static void RemoveFreeArea(CHeader* area)
    {
        if (area->PPrevFreeLink && area->Type == 'N') {
            area->Type = 0;
            *area->PPrevFreeLink = area->PNextFree;
            if (area->PNextFree)
                area->PNextFree->PPrevFreeLink = area->PPrevFreeLink;
            area->PPrevFreeLink = 0;
            area->PNextFree = 0;
        }
    }
};

void CHandleBasedMemoryPool_Methods::CombineAreas(CHeader* first, CHeader* second)
{
    RemoveFreeArea(first);
    RemoveFreeArea(second);
    CHeader* next = second->PNextPhysical;
    first->PNextPhysical = next;
    if (next)
        next->PPrevPhysical = first;
    first->Size += second->Size + sizeof(CHeader);
    first->Type = 'N';
    InsertFreeArea(first);
}