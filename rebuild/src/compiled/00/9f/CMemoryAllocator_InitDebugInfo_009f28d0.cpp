// CMemoryAllocator::InitDebugInfo(void* block, long tag, long size) -- fill the debug trailer of a freshly allocated block: the 12-byte record at block + GetAllocationSize(block) - 0x14 gets {-1, the current allocator index (byte +0x414), size}, the dword just below the block is set to -1, and the per-index byte counter (+0 .. +0x3c, indexed by the byte +0x414) grows by the size
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F28D0, length 80 bytes.
// Leaf: InitDebugInfo
// Callees: GetAllocationSize @ 0x009F2390 (called twice; the first result is discarded). Shape: `this` in esi, the block in edi, `ret 0Ch`.
struct SDebugTrailer_009f28d0 {
    long          m_guard;                         // -1
    unsigned long m_allocatorIndex;
    long          m_size;
};

struct CMemoryAllocator {
    long          m_bytesByIndex[0x104];           // +0x000: per-allocator-index byte counters
    unsigned long m_noSubAllocators;               // +0x410
    unsigned char m_currentIndex;                  // +0x414
    long GetAllocationSize(void* block);           // 0x009F2390
    void InitDebugInfo(void* block, long tag, long size);
};

void CMemoryAllocator::InitDebugInfo(void* block, long tag, long size)
{
    GetAllocationSize(block);
    SDebugTrailer_009f28d0* trailer = (SDebugTrailer_009f28d0*)((char*)block + GetAllocationSize(block) - 0x14);
    trailer->m_guard = -1;
    trailer->m_allocatorIndex = m_currentIndex;
    trailer->m_size = size;
    ((long*)block)[-1] = -1;
    m_bytesByIndex[m_currentIndex] += size;
}