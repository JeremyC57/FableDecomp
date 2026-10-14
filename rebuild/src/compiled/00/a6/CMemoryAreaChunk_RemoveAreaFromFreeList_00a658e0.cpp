// CMemoryAreaChunk::RemoveAreaFromFreeList(SArea* area) -- unlink the area from the chunk's free list (area next at +0, prev at +8; the chunk's head at +0x30), clear its links, decrement the chunk's area count (+8), then re-insert the area into the free list (0x00A650F0) and consolidate its neighbours (0x00A65590)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A658E0, length 80 bytes.
// Leaf: RemoveAreaFromFreeList
// Callees: InsertAreaIntoFreeList @ 0x00A650F0, ConsolidateAdjacentFreeAreas @ 0x00A65590 (both thiscall on the chunk with the area). Shape: the area in esi, `this` in edi, `ret 4`.
struct SArea_00a658e0 {
    SArea_00a658e0* m_pNext;                       // +0x00
    long            m_4;
    SArea_00a658e0* m_pPrev;                       // +0x08
};

struct CMemoryAreaChunk {
    unsigned char   _pad00[8];
    long            m_noAreas;                     // +0x08
    unsigned char   _pad0c[0x24];
    SArea_00a658e0* m_pFirstFreeArea;              // +0x30
    void InsertAreaIntoFreeList(SArea_00a658e0* area);         // 0x00A650F0
    void ConsolidateAdjacentFreeAreas(SArea_00a658e0* area);   // 0x00A65590
    void RemoveAreaFromFreeList(SArea_00a658e0* area);
};

void CMemoryAreaChunk::RemoveAreaFromFreeList(SArea_00a658e0* area)
{
    if (area->m_pPrev)
        area->m_pPrev->m_pNext = area->m_pNext;
    else
        m_pFirstFreeArea = area->m_pNext;
    if (area->m_pNext)
        area->m_pNext->m_pPrev = area->m_pPrev;
    area->m_pNext = 0;
    area->m_pPrev = 0;
    --m_noAreas;
    InsertAreaIntoFreeList(area);
    ConsolidateAdjacentFreeAreas(area);
}