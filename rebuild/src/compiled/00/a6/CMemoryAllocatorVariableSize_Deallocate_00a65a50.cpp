// CMemoryAllocatorVariableSize::Deallocate(void* block) -- publish the two usage counters to the statistics globals, find the area header (block minus the aligned header size) and its chunk (+0x14), remove the area from the chunk's free list (0x00A658E0), and when the chunk has no areas left (+8) and is not pinned (byte +0x1c) release the chunk (0x00A65390)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A65A50, length 78 bytes.
// Leaf: ?Deallocate@CMemoryAllocatorVariableSize@@UAEXPAX@Z
// Callees: CMemoryAreaChunk::RemoveAreaFromFreeList @ 0x00A658E0 (thiscall on the chunk), the chunk release @ 0x00A65390 (thiscall on `this`). Shape: `this` in edi, the chunk in esi, `ret 4`.
struct SVarChunk_00a65a50;
struct SAreaHeader_00a65a50 {
    unsigned char       _pad00[0x14];
    SVarChunk_00a65a50* m_pChunk;                  // +0x14
};
struct SVarChunk_00a65a50 {
    unsigned char _pad00[8];
    long          m_noAreas;                       // +0x08
    unsigned char _pad0c[0x10];
    bool          m_pinned;                        // +0x1c
    void RemoveAreaFromFreeList(SAreaHeader_00a65a50* area);   // 0x00A658E0
};
extern unsigned long g_LastAllocatorChunkCounter_013d28d8;   // 0x013D28D8
extern unsigned long g_LastAllocatorUsage_013d28d4;          // 0x013D28D4

struct CMemoryAllocatorVariableSize {
    virtual void v000();
    unsigned char _pad04[0xc14 - 4];
    unsigned long m_usage;                         // +0xc14: the header size
    unsigned long m_alignmentShift;                // +0xc18
    void ReleaseChunk(SVarChunk_00a65a50* chunk);  // 0x00A65390
    virtual void Deallocate(void* block);
};

void CMemoryAllocatorVariableSize::Deallocate(void* block)
{
    unsigned long shift = m_alignmentShift;
    g_LastAllocatorChunkCounter_013d28d8 = shift;
    unsigned long usage = m_usage;
    g_LastAllocatorUsage_013d28d4 = usage;
    SAreaHeader_00a65a50* area = (SAreaHeader_00a65a50*)((char*)block - (((usage + 0x1f) >> shift) << shift));
    SVarChunk_00a65a50* chunk = area->m_pChunk;
    chunk->RemoveAreaFromFreeList(area);
    if (chunk->m_noAreas == 0 && !chunk->m_pinned)
        ReleaseChunk(chunk);
}