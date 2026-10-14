// CMemoryAllocatorFixedSize::Validate(SChunk* chunk) -- validate every area of one chunk (first at +0x14; the area header's own index at +0, the next index at +1, 0xFF ending the chain; step = (next - index) * the stride at +0xc2c): each payload (header + 4) is checked by the global allocator (0x013CAA90, 0x009F1F40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A66260, length 64 bytes.
// Leaf: Validate
// Callees: 0x009F1F40 (thiscall on the global allocator @ 0x013CAA90, one argument). Shape: the area in esi, `this` in edi, the loop head aligned with a `nop`, `ret 4`.
struct SFixedChunk_00a66260 {                      // a chunk of the fixed-size allocator: next at +8, the first area at +0x14
    unsigned char      _pad00[8];
    SFixedChunk_00a66260* m_pNext;                    // +0x08
    unsigned char      _pad0c[8];
    unsigned char*     m_pFirstArea;               // +0x14
};
struct CMemoryAllocator_00a66260 {
    void ValidateBlock(void* block);               // 0x009F1F40
};
extern CMemoryAllocator_00a66260* g_pGlobalAllocator_013caa90;   // 0x013CAA90

struct CMemoryAllocatorFixedSize {
    unsigned char _pad00[0xc2c];
    long          m_areaStride;                    // +0xc2c
    void Validate(SFixedChunk_00a66260* chunk);
};

void CMemoryAllocatorFixedSize::Validate(SFixedChunk_00a66260* chunk)
{
    unsigned char* area = chunk->m_pFirstArea;
    while (area) {
        g_pGlobalAllocator_013caa90->ValidateBlock(area + 4);
        unsigned char next = area[1];
        if (next == 0xff)
            break;
        area += (next - area[0]) * m_areaStride;
    }
}
