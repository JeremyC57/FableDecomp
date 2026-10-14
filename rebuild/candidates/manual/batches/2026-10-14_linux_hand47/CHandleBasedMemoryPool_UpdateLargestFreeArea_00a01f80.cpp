// CHandleBasedMemoryPool::UpdateLargestFreeArea() -- recompute LargestFreeArea: walk the size buckets downwards from the current largest's bucket to the first non-empty one and take the biggest area in it; zero when every bucket is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A01F80, length 85 bytes.
// Callees: none. Shape: the bucket index unsigned-capped then walked as a signed counter (`test eax,eax ; jl`) alongside a strength-reduced bucket pointer (`sub edx,4`), the found bucket re-read
// by index, the running maximum in esi (`cmp esi,edx ; jge`), the zero store on the empty path.
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
    void UpdateLargestFreeArea();
};

void CHandleBasedMemoryPool_Methods::UpdateLargestFreeArea()
{
    unsigned long bucket = (unsigned long)LargestFreeArea >> 9;
    if (bucket >= 255)
        bucket = 255;
    for (long index = (long)bucket; index >= 0; --index) {
        if (FreeLists[index]) {
            long largest = 0;
            for (CHeader* area = FreeLists[index]; area; area = area->PNextFree) {
                if (largest < area->Size)
                    largest = area->Size;
            }
            LargestFreeArea = largest;
            return;
        }
    }
    LargestFreeArea = 0;
}
