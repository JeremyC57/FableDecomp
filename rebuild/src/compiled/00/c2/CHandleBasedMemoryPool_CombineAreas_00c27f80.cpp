// CHandleBasedMemoryPool_012b4098::CombineAreas(CHeader* first, CHeader* second) -- merge two physically adjacent free areas: take both off the free lists, splice the second out of the physical chain, grow the first by the second's size plus its header, mark it free, re-insert it and return it
// Leaf: ?CombineAreas@CHandleBasedMemoryPool_Methods@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C27F80, 74 bytes (`ret 8`). The libpk build of the engine's
//   CHandleBasedMemoryPool_012b4098::CombineAreas (engine copy 0x00A01E40, which returns void and inlines the free-list unlink);
//   the manifest's "CHandleBasedMemoryPool_MergeBlocks" is descriptive.
// Callees: RemoveFreeArea @ 0x00C27F50 (out of line here, landed), InsertFreeArea @ 0x00C27EF0 (landed).
// Shape: `this` in edi with `mov ecx,edi` before each call; the size grows by `second->Size + sizeof(CHeader)`
//   (the 0x20 added to the second size first); the 'N' store sits between the push and the InsertFreeArea call.
struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader, 32 bytes in front of every area (landed 0x00C27F50 names)
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10 (the free-list link that points at this area)
    CHeader*       NextAvailable;               // +0x14
    CHeader*       PrevContiguous;              // +0x18
    CHeader*       NextContiguous;              // +0x1c
};
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    void     RemoveFreeArea(CHeader* area);    // 0x00C27F50
    void     InsertFreeArea(CHeader* area);    // 0x00C27EF0
    CHeader* CombineAreas(CHeader* first, CHeader* second);
};

CHeader* CHandleBasedMemoryPool_Methods::CombineAreas(CHeader* first, CHeader* second)
{
    RemoveFreeArea(first);
    RemoveFreeArea(second);
    CHeader* next = second->NextContiguous;
    first->NextContiguous = next;
    if (next)
        next->PrevContiguous = first;
    first->Size += second->Size + sizeof(CHeader);
    first->Status = 'N';
    InsertFreeArea(first);
    return first;
}