// CHandleBasedMemoryPool_012b4098::ResizeArea(CHeader* area, long newSize) -- shrink an area to newSize: more than a header's worth of spare bytes becomes a new free area after it, otherwise the spare is kept as the area's padding
// Leaf: ?ResizeArea@CHandleBasedMemoryPool_Methods@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28290, 71 bytes (`ret 8`). The libpk build of the engine's
//   CHandleBasedMemoryPool_012b4098::ResizeArea (engine copy 0x00A02410: an unsigned spare test and the header built inline);
//   the manifest's "Thiscall_BlockAllocationShrink" is propagated.
// Callees: CreateArea @ 0x00C27EA0 (at `area + newSize + 0x20`, the spare bytes, between the area and its physical
//   successor), InsertFreeArea @ 0x00C27EF0.
// Shape: the spare is SIGNED here (`cmp eax,20h ; jle`); each arm stores the new size and has its own epilogue, the
//   small-spare arm then the padding word.
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
    CHeader* CreateArea(void* address, unsigned long size, CHeader* prev, CHeader* next);   // 0x00C27EA0
    void     InsertFreeArea(CHeader* area);    // 0x00C27EF0
    void     ResizeArea(CHeader* area, long newSize);
};

void CHandleBasedMemoryPool_Methods::ResizeArea(CHeader* area, long newSize)
{
    long spare = area->Size - newSize;
    if (spare > (long)sizeof(CHeader)) {
        InsertFreeArea(CreateArea((char*)area + newSize + sizeof(CHeader), spare, area, area->NextContiguous));
        area->Size = newSize;
    } else {
        area->Size = newSize;
        area->Padding = (short)spare;
    }
}