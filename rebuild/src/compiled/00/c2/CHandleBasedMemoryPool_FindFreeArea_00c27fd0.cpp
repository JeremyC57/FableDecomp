// CHandleBasedMemoryPool_012b4098::FindFreeArea(long size) -- best-fit search (second pool build): walk the free lists from the size's bucket (size >> 9, capped at 255) upwards and return the smallest free area that holds `size`, stopping early on an exact fit; null when none
// Leaf: FindFreeArea
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27FD0, length 116 bytes. No callees.
// This is the 0x00C27xxx-0x00C28xxx copy of the pool (FindLargestFreeArea 0x00C28050, InsertFreeArea 0x00C27EF0
// landed): its free lists sit at +0x3c (one word later than the 0x00A0xxxx build) and the area test has no
// flags byte. Shape: the bucket clamp is a MIN-style ternary that spells `size >> 9` twice (a macro expansion) as the
// loop index initialiser (out-of-line `mov ebx,0FFh ; jmp`); a named bucket local lets VC7.1 test the copy (edx) instead
// of the index (ebx) at the loop entry; best size 0x7fffffff in esi, result in eax re-tested at the outer loop head.
struct SFreeArea_00c27fd0 {                        // a pool area record (as landed with 0x00C28050)
    void*                m_pResource;              // +0x00
    unsigned char        _pad04[4];
    long                 m_size;                   // +0x08
    short                m_padding;                // +0x0c
    unsigned char        m_state;                  // +0x0e ('T' = taken, 'N' = free)
    unsigned char        _pad0f;
    SFreeArea_00c27fd0** m_pPrevLink;              // +0x10
    SFreeArea_00c27fd0*  m_pNext;                  // +0x14
};
class CHandleBasedMemoryPool_012b4098 { public:
    unsigned char       _pad[0x3c];
    SFreeArea_00c27fd0* m_freeLists[256];          // +0x3c
    SFreeArea_00c27fd0* FindFreeArea(long size);
};

SFreeArea_00c27fd0* CHandleBasedMemoryPool_012b4098::FindFreeArea(long size)
{
    SFreeArea_00c27fd0* best = 0;
    long bestSize = 0x7fffffff;
    for (unsigned long i = ((unsigned long)size >> 9) < 255 ? ((unsigned long)size >> 9) : 255; i < 256 && !best; ++i) {
        for (SFreeArea_00c27fd0* area = m_freeLists[i]; area; area = area->m_pNext) {
            if (size <= area->m_size && area->m_size < bestSize) {
                bestSize = area->m_size;
                best = area;
                if (bestSize == size)
                    break;
            }
        }
    }
    return best;
}