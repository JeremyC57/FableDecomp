// CHandleBasedMemoryPool::FindLargestFreeArea() -- walk the free list of the bucket holding the largest-free-area size (+0x1c; bucket = size >> 9 capped at 255) and return the first area of exactly that size; null when none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28050, length 48 bytes.
// Shape: `shr eax,9 ; cmp eax,0FFh ; jb`, the aligned loop over `[eax+14h]`, `xor eax,eax`.
struct SFreeArea_00c28050 {                           // a pool area record: resource word, size, padding, the free-list links
    void*            m_pResource;                  // +0x00
    unsigned char    _pad04[4];
    long             m_size;                       // +0x08
    short            m_padding;                    // +0x0c
    unsigned char    m_state;                      // +0x0e ('T' = taken, 'N' = free)
    unsigned char    _pad0f;
    SFreeArea_00c28050** m_pPrevLink;                 // +0x10 (the link that points at this area)
    SFreeArea_00c28050*  m_pNext;                     // +0x14
};

class CHandleBasedMemoryPool { public:
    unsigned char       _pad[0x1c];
    long                m_largestFreeArea;         // +0x1c
    unsigned char       _pad20[0x1c];
    SFreeArea_00c28050* m_freeLists[256];          // +0x3c
    SFreeArea_00c28050* FindLargestFreeArea();
};

SFreeArea_00c28050* CHandleBasedMemoryPool::FindLargestFreeArea()
{
    long size = m_largestFreeArea;
    unsigned long bucket = (unsigned long)size >> 9;
    if (bucket >= 0xff)
        bucket = 0xff;
    for (SFreeArea_00c28050* area = m_freeLists[bucket]; area; area = area->m_pNext) {
        if (area->m_size == size)
            return area;
    }
    return 0;
}