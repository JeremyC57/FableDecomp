// CVector_ulong_00c29b30::push_back(const unsigned long& item) -- append to a VC7-era Dinkumware vector<unsigned long> (allocator word +0, begin +4, end +8, capacity end +0xc): in place when the size is below the capacity, else through the growing InsertRange (0x00C295B0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29B30, length 77 bytes.
// Callees: InsertRange @ 0x00C295B0 (thiscall, (where, count, item), `ret 0Ch`). The chunk label (CMirroredTextureHighlightsChunk::AddVertexIndex) is propagated: `this` has the begin pointer at +4 (the chunk's own TriangleVertexIndexList keeps it at +0x10), so the body is the vector's own push_back (`size() < capacity()` with the null-begin tests of the Dinkumware accessors, `_Insert_n(end, 1, item)` otherwise); named by address. Shape: the size (null begin -> 0) in edx, the capacity (null begin -> the slow path) in eax, an unsigned compare, `ret 4`.
struct CVector_ulong_00c29b30 {
    long           m_allocator;                    // +0x00 (the empty allocator padded to a word)
    unsigned long* m_pBegin;                       // +0x04
    unsigned long* m_pEnd;                         // +0x08
    unsigned long* m_pCapacityEnd;                 // +0x0c
    unsigned long GetSize() const { return m_pBegin == 0 ? 0 : (unsigned long)(m_pEnd - m_pBegin); }
    unsigned long GetCapacity() const { return m_pBegin == 0 ? 0 : (unsigned long)(m_pCapacityEnd - m_pBegin); }
    static unsigned long* Fill(unsigned long* where, const unsigned long& item) { *where = item; return where + 1; }   // _Ufill(_Mylast, 1, _Val): the new end comes back through a register
    struct iterator {                              // the unchecked VC7 vector iterator: one pointer, no user copy constructor (pushed as a plain word)
        unsigned long* m_ptr;
        iterator(unsigned long* p) : m_ptr(p) { }
    };
    iterator end() { return iterator(m_pEnd); }
    void InsertRange(iterator where, unsigned long count, const unsigned long& item);   // 0x00C295B0 (_Insert_n)
    void push_back(const unsigned long& item);
};

void CVector_ulong_00c29b30::push_back(const unsigned long& item)
{
    if (GetSize() < GetCapacity())
        m_pEnd = Fill(m_pEnd, item);
    else
        InsertRange(end(), 1, item);
}
