// CMemoryAllocatorFixedSize::GetAllAllocatedAreas(void** areas, unsigned long max) -- after a consolidation (virtual slot 9, +0x24), walk every chunk (+0xc1c, next at +8) and every area of the chunk (first at +0x14; each area header holds its own index byte at +0 and the next index at +1, 0xFF ending the chain; the step is (next - index) * the area stride at +0xc2c), storing each area's payload address (header + 4) into the array; returns the count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A660A0, length 96 bytes.
// Leaf: GetAllAllocatedAreas
// Shape: the count in esi, the array in ebx, the stride multiply with the index difference, the area loop ending on a 0xFF next-index or a zero step, `ret 8`.
struct SFixedChunk_00a660a0 {                      // a chunk of the fixed-size allocator: next at +8, the first area at +0x14
    unsigned char      _pad00[8];
    SFixedChunk_00a660a0* m_pNext;                    // +0x08
    unsigned char      _pad0c[8];
    unsigned char*     m_pFirstArea;               // +0x14
};

struct CMemoryAllocatorFixedSize {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void ConsolidateFreeAreas();           // slot 9 (+0x24)
    unsigned char        _pad04[0xc1c - 4];
    SFixedChunk_00a660a0* m_pFirstChunk;           // +0xc1c
    unsigned char        _padc20[0xc];
    long                 m_areaStride;             // +0xc2c
    unsigned long GetAllAllocatedAreas(void** areas, unsigned long max);
};

unsigned long CMemoryAllocatorFixedSize::GetAllAllocatedAreas(void** areas, unsigned long max)
{
    unsigned long count = 0;
    ConsolidateFreeAreas();
    for (SFixedChunk_00a660a0* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext) {
        unsigned char* area = chunk->m_pFirstArea;
        while (area) {
            areas[count++] = area + 4;
            unsigned char next = area[1];
            if (next == 0xff)
                break;
            area += (next - area[0]) * m_areaStride;
        }
    }
    return count;
}