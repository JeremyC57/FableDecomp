// std::vector<CFileInstaller::CBufferChunk>::erase(first, last) -- the Dinkumware range erase over 8-byte chunks: copy [last, end) down to `first` (two words per element), destroy the tail (0x009C6D60), shrink, return `first`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C7150, length 80 bytes.
// Callees: `_Destroy_range<std::allocator<CFileInstaller::CBufferChunk>>` @ 0x009C6D60 (`__fastcall`: the new end in ecx, the old end in edx, a default-constructed
// allocator temporary bound to the `const&` parameter by address -- the temporary lives in the dead `first` argument slot). Shape: the element count `(end - last) >> 3`, the copy loop addressing the
// source as `[dest + (last - first)]`, `ret 8`. The manifest label "CFileInstaller::CFileInstaller::RemoveRange" is the enclosing class's.
struct CBufferChunk_009c7150 { long m_a; long m_b; };   // 8 bytes

struct CRetailAllocator_009c7150 {
    CRetailAllocator_009c7150() { }
    CRetailAllocator_009c7150(const CRetailAllocator_009c7150&) { }   // user copy constructor + destructor: passed by the address of a temporary
    ~CRetailAllocator_009c7150() { }
};
void __fastcall DestroyRange_009c6d60(CBufferChunk_009c7150* first, CBufferChunk_009c7150* last, const CRetailAllocator_009c7150& allocator);   // 0x009C6D60

struct CRetailVector_CBufferChunk {
    CBufferChunk_009c7150* m_pFirst;
    CBufferChunk_009c7150* m_pLast;
    CBufferChunk_009c7150* m_pEnd;
    static CBufferChunk_009c7150* Copy(CBufferChunk_009c7150* f, CBufferChunk_009c7150* l, CBufferChunk_009c7150* result)
    {
        for (long n = l - f; n > 0; --n) {
            *result = *f;
            ++f;
            ++result;
        }
        return result;
    }
    CBufferChunk_009c7150* erase(CBufferChunk_009c7150* first, CBufferChunk_009c7150* last);
};

CBufferChunk_009c7150* CRetailVector_CBufferChunk::erase(CBufferChunk_009c7150* first, CBufferChunk_009c7150* last)
{
    CBufferChunk_009c7150* newEnd = Copy(last, m_pLast, first);
    DestroyRange_009c6d60(newEnd, m_pLast, CRetailAllocator_009c7150());
    m_pLast = newEnd;
    return first;
}
