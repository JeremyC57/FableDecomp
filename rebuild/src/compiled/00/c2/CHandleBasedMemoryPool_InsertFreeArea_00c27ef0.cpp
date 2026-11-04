// CHandleBasedMemoryPool_012b4098::InsertFreeArea_00c27ef0(SFreeArea* area) -- fold the area's padding into its size (a compound add on the member: the padding converted first), push it on the free list for its size bucket (size >> 9, capped at 255; the lists at +0x3c) with the back link maintained, and raise the largest-free-area word (+0x1c) when it is bigger
// Vendor naming wave V1 (one address, one name): 0x00C27EF0 ?InsertFreeArea_00c27ef0@CHandleBasedMemoryPool_012b4098@@QAEXPAUCHeader@@@Z (was ?InsertFreeArea@CHandleBasedMemoryPool_012b4098@@QAEXPAUSFreeArea_00c27ef0@@@Z)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C27EF0, length 96 bytes.
// Shape: `movsx` of the padding short, `shr edx,9 ; cmp edx,0FFh ; jb`, `lea edx,[ecx+edx*4+3Ch]`, `ret 4`.
struct CHeader {                           // a pool area record: resource word, size, padding, the free-list links
    void*            m_pResource;                  // +0x00
    unsigned char    _pad04[4];
    long             m_size;                       // +0x08
    short            m_padding;                    // +0x0c
    unsigned char    m_state;                      // +0x0e ('T' = taken, 'N' = free)
    unsigned char    _pad0f;
    CHeader** m_pPrevLink;                 // +0x10 (the link that points at this area)
    CHeader*  m_pNext;                     // +0x14
};

class CHandleBasedMemoryPool_012b4098 { public:
    unsigned char       _pad[0x1c];
    long                m_largestFreeArea;         // +0x1c
    unsigned char       _pad20[0x1c];
    CHeader* m_freeLists[256];          // +0x3c (by size bucket)
    void InsertFreeArea_00c27ef0(CHeader* area);
};

void CHandleBasedMemoryPool_012b4098::InsertFreeArea_00c27ef0(CHeader* area)
{
    area->m_size += area->m_padding;
    long size = area->m_size;
    unsigned long bucket = (unsigned long)size >> 9;
    area->m_padding = 0;
    if (bucket >= 0xff)
        bucket = 0xff;
    CHeader** head = &m_freeLists[bucket];
    area->m_pPrevLink = head;
    CHeader* old = *head;
    area->m_pNext = old;
    if (old)
        old->m_pPrevLink = &area->m_pNext;
    *head = area;
    if (m_largestFreeArea < area->m_size)
        m_largestFreeArea = area->m_size;
}