// CHandleBasedMemoryPool::CombineAdjacentAreas(CHeader** pArea) -- merge the area with its free physical successor, then with its free physical predecessor (the merged area becomes the predecessor, written back through the pointer)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A02470, length 80 bytes.
// Callees: CHandleBasedMemoryPool::CombineAreas(CHeader* first, CHeader* second) @ 0x00A01E40 (twice, on `this`). Shape: the area re-read through the argument after the first merge,
// the type byte compared with 'N' (0x4E), `this` in ebp, the predecessor in esi and stored back, `ret 4`.
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
    void CombineAreas(CHeader* first, CHeader* second);   // 0x00A01E40
    void CombineAdjacentAreas(CHeader** pArea);
};

void CHandleBasedMemoryPool_Methods::CombineAdjacentAreas(CHeader** pArea)
{
    CHeader* next = (*pArea)->PNextPhysical;
    if (next && next->Type == 'N')
        CombineAreas(*pArea, next);
    CHeader* prev = (*pArea)->PPrevPhysical;
    if (prev && prev->Type == 'N') {
        CombineAreas(prev, *pArea);
        *pArea = prev;
    }
}
