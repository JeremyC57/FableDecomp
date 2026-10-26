// CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(long size) -- search the chunk tree (root +0xc34, left +4, right +0x10, key = the largest free size at +0x18) for the best fit: an exact key returns that chunk at once, otherwise the smallest chunk whose key exceeds the size (descending right when the key is larger, left otherwise)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A64C10, length 55 bytes.
// Leaf: AllocateMemoryFromExistingChunks
// The retail label names the caller's intent; the body is the best-fit search. Shape: the candidate in edx, the key in ecx, `ret 4`.
struct SVarChunk_00a64c10 {                        // the chunk tree node: left +4, right +0x10, key +0x18
    long                m_0;
    SVarChunk_00a64c10* m_pLeft;                   // +0x04
    unsigned char       _pad08[8];
    SVarChunk_00a64c10* m_pRight;                  // +0x10
    long                m_14;
    long                m_largestFree;             // +0x18
};

class CMemoryAllocatorVariableSize { public:
    unsigned char       _pad00[0xc34];
    SVarChunk_00a64c10* m_pChunkTreeRoot;          // +0xc34
    SVarChunk_00a64c10* AllocateMemoryFromExistingChunks(long size);
};

SVarChunk_00a64c10* CMemoryAllocatorVariableSize::AllocateMemoryFromExistingChunks(long size)
{
    SVarChunk_00a64c10* best = 0;
    for (SVarChunk_00a64c10* node = m_pChunkTreeRoot; node; ) {
        long key = node->m_largestFree;
        if (key == size)
            return node;
        if (key > size) {
            if (!best || key < best->m_largestFree)
                best = node;
            node = node->m_pRight;
        } else {
            node = node->m_pLeft;
        }
    }
    return best;
}