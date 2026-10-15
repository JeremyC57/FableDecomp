// CVertexPairContraction::RemapIndex(unsigned long from, unsigned long to) -- replace every occurrence of vertex index `from` by `to` in both halves of the pair vector
// Grade: DIFFER(ours v retail) -- 76v75: the lane loads m_pFirst into a register before the entry `size` test (retail `mov eax,[ecx+8] ; sub eax,[ecx+4]` from memory) and compares the first field from memory while loading the second (retail loads the first into ebx, `lea` once, compares the second from memory); tried: pointer `m_pFirst + i`, `*(Begin() + i)` reference, `volatile` m_pFirst, STLport operator[] per access
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC7940, length 80 bytes. No callees.
// Shape: the size is `(m_pLast - m_pFirst)` over 8-byte elements (`sub ; sar eax,3`) tested and compared
// UNSIGNED (`jbe`, `jb`) and recomputed every iteration (an inline Size()); the element address is formed
// once (`lea eax,[eax+edx*8]`) for both field tests, `from` in esi, `to` in edi; `ret 8`.
// Layout: a 12-byte retail vector of {first, second} index pairs at +4 (the vptr at +0); no engine header.
struct SIndexPair {
    unsigned long m_first;
    unsigned long m_second;
};

struct CVertexPairContraction {
    virtual void v00();
    SIndexPair* m_pFirst;                      // +0x04
    SIndexPair* m_pLast;                       // +0x08
    SIndexPair* m_pEnd;                        // +0x0c
    SIndexPair* Begin() { return m_pFirst; }
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pFirst); }
    void RemapIndex(unsigned long from, unsigned long to);
};

void CVertexPairContraction::RemapIndex(unsigned long from, unsigned long to)
{
    for (unsigned long i = 0; i < Size(); ++i) {
        SIndexPair& pair = *(Begin() + i);       // STLport operator[]: `*(begin() + n)` through an inline begin()
        if (pair.m_first == from)
            pair.m_first = to;
        if (pair.m_second == from)
            pair.m_second = to;
    }
}
