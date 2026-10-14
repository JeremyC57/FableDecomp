// CMemoryAllocatorFixedSize::GetAllocatedArea(long index) -- walk every chunk (+0xc1c, next at +8) and every area of the chunk (first at +0x14; the area header's own index at +0, the next index at +1, 0xFF ending the chain; step = (next - index) * the stride at +0xc2c) counting areas; the payload (header + 4) of the index-th area, or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66100, length 96 bytes.
// Leaf: GetAllocatedArea
// Shape: the running count in esi compared with the index (ebx) at the top of the inner loop, the found exit adds 4 to the header after the pops, `ret 4`.
struct SFixedChunk_00a66100 {                      // a chunk of the fixed-size allocator: next at +8, the first area at +0x14
    unsigned char      _pad00[8];
    SFixedChunk_00a66100* m_pNext;                    // +0x08
    unsigned char      _pad0c[8];
    unsigned char*     m_pFirstArea;               // +0x14
};

struct CMemoryAllocatorFixedSize {
    unsigned char        _pad00[0xc1c];
    SFixedChunk_00a66100* m_pFirstChunk;           // +0xc1c
    unsigned char        _padc20[0xc];
    long                 m_areaStride;             // +0xc2c
    void* GetAllocatedArea(long index);
};

void* CMemoryAllocatorFixedSize::GetAllocatedArea(long index)
{
    long count = 0;
    for (SFixedChunk_00a66100* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) {
        unsigned char* area = chunk->m_pFirstArea;
        while (area) {
            if (count == index)
                return area + 4;
            unsigned char next = area[1];
            ++count;
            if (next == 0xff)
                break;
            area += (next - area[0]) * m_areaStride;
        }
    }
    return 0;
}