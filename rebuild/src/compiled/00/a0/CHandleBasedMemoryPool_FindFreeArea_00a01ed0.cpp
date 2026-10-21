// CHandleBasedMemoryPool::FindFreeArea(long size) -- best-fit search: walk the free lists from the size's bucket (size >> 9, capped at 255) upwards and return the smallest untouched free area that holds `size` (stop early on an exact fit); null when none
// Leaf: FindFreeArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01ED0, length 122 bytes. No callees.
// Shape: the best size starts at 0x7fffffff (esi) and the result at 0 (eax); the outer bucket loop is rotated
// (`cmp ebx,100h ; jae` before it) and re-tests the result at its head (`test eax,eax ; jne`), the bucket list
// address walks in ebp. The area test reads `size <= area->Size` (`cmp edi,edx ; jg`), `Size < best` and the
// flags byte at +0xf; an exact fit breaks the inner walk. The bucket clamp is a MIN-style ternary spelling `size >> 9`
// twice (a macro expansion): over a named bucket local VC7.1 tests the copy (edx) at loop entry instead of the index (ebx).
// Model: engine/CHandleBasedMemoryPool.h (FreeLists[256] at +0x38 agrees with the bytes); CHeader as landed with
// InsertFreeArea (0x00A01DA0).
struct CHeader {                                  // CHandleBasedMemoryPool::CHeader, 32 bytes in front of every area
    long           Handle;                         // +0x00
    long           _unused04;                      // +0x04
    long           Size;                           // +0x08
    short          Padding;                        // +0x0c
    char           Type;                           // +0x0e ('N' free, 'T' taken)
    char           Flags;                          // +0x0f
    CHeader**      PPrevFreeLink;                  // +0x10
    CHeader*       PNextFree;                      // +0x14
    CHeader*       PPrevPhysical;                  // +0x18
    CHeader*       PNextPhysical;                  // +0x1c
};
#include "engine/CHandleBasedMemoryPool.h"

struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    CHeader* FindFreeArea(long size);
};

CHeader* CHandleBasedMemoryPool_Methods::FindFreeArea(long size)
{
    CHeader* best = 0;
    long bestSize = 0x7fffffff;
    for (unsigned long i = ((unsigned long)size >> 9) < 255 ? ((unsigned long)size >> 9) : 255; i < 256 && !best; ++i) {
        for (CHeader* area = FreeLists[i]; area; area = area->PNextFree) {
            if (size <= area->Size && area->Size < bestSize && area->Flags == 0) {
                bestSize = area->Size;
                best = area;
                if (bestSize == size)
                    break;
            }
        }
    }
    return best;
}