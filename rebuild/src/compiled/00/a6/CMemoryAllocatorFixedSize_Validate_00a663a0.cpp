// CMemoryAllocatorFixedSize::Validate() -- validate every chunk of the list (+0xc1c, next at +8) through the per-chunk Validate (0x00A66260)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A663A0, length 48 bytes.
// Leaf: ?Validate@CMemoryAllocatorFixedSize@@UAEXXZ
// Callees: Validate(chunk) @ 0x00A66260. Shape: the chunk in esi, `this` in edi, the loop head aligned with `mov edi,edi`, `ret`.
struct SFixedChunk_00a663a0 {                      // a chunk of the fixed-size allocator: next at +8, the first area at +0x14
    unsigned char      _pad00[8];
    SFixedChunk_00a663a0* m_pNext;                    // +0x08
    unsigned char      _pad0c[8];
    unsigned char*     m_pFirstArea;               // +0x14
};

struct CMemoryAllocatorFixedSize {
    virtual void v000();
    unsigned char        _pad04[0xc1c - 4];
    SFixedChunk_00a663a0* m_pFirstChunk;           // +0xc1c
    void Validate(SFixedChunk_00a663a0* chunk);    // 0x00A66260
    virtual void Validate();
};

void CMemoryAllocatorFixedSize::Validate()
{
    for (SFixedChunk_00a663a0* chunk = m_pFirstChunk; chunk; chunk = chunk->m_pNext)
        Validate(chunk);
}