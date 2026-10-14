// CMemoryAllocatorFixedSize::DeleteChunk(SChunk* chunk) -- unlink a chunk from the allocator's doubly linked chunk list (+8 prev, +0xc next): the neighbours re-linked, then the head (+0xc1c) and tail (+0xc20) advanced when they pointed at it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65E70, length 80 bytes.
// Leaf: DeleteChunk
// Shape: no frame, the chunk in eax, `ret 4`.
struct SFixedChunk_00a65e70 {                      // a chunk of the fixed-size allocator: a doubly linked list (+8 prev, +0xc next), areas at +0x14
    unsigned char       _pad00[8];
    SFixedChunk_00a65e70* m_pPrev;                 // +0x08
    SFixedChunk_00a65e70* m_pNext;                 // +0x0c
    unsigned char       _pad10[4];
    unsigned char*      m_pFirstArea;              // +0x14
};

struct CMemoryAllocatorFixedSize {
    unsigned char       _pad00[0xc1c];
    SFixedChunk_00a65e70* m_pFirstChunk;           // +0xc1c
    SFixedChunk_00a65e70* m_pLastChunk;            // +0xc20
    void DeleteChunk(SFixedChunk_00a65e70* chunk);
};

void CMemoryAllocatorFixedSize::DeleteChunk(SFixedChunk_00a65e70* chunk)
{
    if (chunk->m_pPrev)
        chunk->m_pPrev->m_pNext = chunk->m_pNext;
    if (chunk->m_pNext)
        chunk->m_pNext->m_pPrev = chunk->m_pPrev;
    if (m_pFirstChunk == chunk)
        m_pFirstChunk = chunk->m_pPrev;
    if (m_pLastChunk == chunk)
        m_pLastChunk = chunk->m_pNext;
}