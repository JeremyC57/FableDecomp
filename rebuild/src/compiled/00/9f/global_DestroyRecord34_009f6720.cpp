// _global::_Destroy_Record34(SRecord34* first, SRecord34* last) -- call the (empty, out-of-line) element destructor over [first, last) of 52-byte records
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6720, length 33 bytes (padded to 48).
// Leaf: _Destroy_Record34
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is STLport
// `_Destroy(first, last)` emitted out of line (first in ecx, last in edx, no tag -> plain `ret`) over 0x34-byte
// elements whose destructor is declared out of line: 0x00A03B30 is a bare `ret` (an empty destructor folded with every
// other empty member by /OPT:ICF, landed as "$E2"), so the call survives although it does nothing.
// Callees: element destructor @ 0x00A03B30 (empty).
// Shape: guarded do-while, `last` in edi, `mov ecx,esi ; call` per element, stride 0x34.
struct SRecord34 {                          // 52-byte element
    long m_words[13];
    ~SRecord34();                           // 0x00A03B30 (empty body, ICF-folded)
};

void __fastcall _Destroy_Record34(SRecord34* first, SRecord34* last)
{
    for (; first != last; ++first)
        first->~SRecord34();
}