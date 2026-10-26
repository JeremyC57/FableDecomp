// CHandleBasedMemoryPool::UpdateLargestFreeArea() -- find the highest non-empty free-list bucket at or below the one of the current largest size (+0x1c) and set the largest-free-area word to the biggest size on that list; zero when every bucket down to 0 is empty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C28080, length 96 bytes.
// Shape: the bucket scan downwards by `sub edx,4`, then the max scan over `[eax+8]` in esi.
struct SFreeArea_00c28080 {                           // a pool area record: resource word, size, padding, the free-list links
    void*            m_pResource;                  // +0x00
    unsigned char    _pad04[4];
    long             m_size;                       // +0x08
    short            m_padding;                    // +0x0c
    unsigned char    m_state;                      // +0x0e ('T' = taken, 'N' = free)
    unsigned char    _pad0f;
    SFreeArea_00c28080** m_pPrevLink;                 // +0x10 (the link that points at this area)
    SFreeArea_00c28080*  m_pNext;                     // +0x14
};

class CHandleBasedMemoryPool { public:
    unsigned char       _pad[0x1c];
    long                m_largestFreeArea;         // +0x1c
    unsigned char       _pad20[0x1c];
    SFreeArea_00c28080* m_freeLists[256];          // +0x3c
    void UpdateLargestFreeArea();
};

void CHandleBasedMemoryPool::UpdateLargestFreeArea()
{
    unsigned long cap = (unsigned long)m_largestFreeArea >> 9;
    if (cap >= 0xff)
        cap = 0xff;
    for (long bucket = (long)cap; bucket >= 0; --bucket) {
        if (m_freeLists[bucket]) {
            long largest = 0;
            for (SFreeArea_00c28080* area = m_freeLists[bucket]; area; area = area->m_pNext) {
                if (largest < area->m_size)
                    largest = area->m_size;
            }
            m_largestFreeArea = largest;
            return;
        }
    }
    m_largestFreeArea = 0;
}