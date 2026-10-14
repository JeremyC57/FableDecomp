// CHandleBasedMemoryPool::FindLargestFreeArea() -- the free area whose size is exactly LargestFreeArea, searched in its size bucket (size >> 9, capped at 255); null when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01F50, length 48 bytes.
// Callees: none. Shape: the size kept in edx, the bucket index unsigned (`shr ; cmp ,0FFh ; jb`), the aligned loop over PNextFree (`lea esp,[esp]` pad), the found area returned in eax.
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
    CHeader* FindLargestFreeArea();
};

CHeader* CHandleBasedMemoryPool_Methods::FindLargestFreeArea()
{
    long size = LargestFreeArea;
    unsigned long bucket = (unsigned long)size >> 9;
    if (bucket >= 255)
        bucket = 255;
    for (CHeader* area = FreeLists[bucket]; area; area = area->PNextFree) {
        if (area->Size == size)
            return area;
    }
    return 0;
}
