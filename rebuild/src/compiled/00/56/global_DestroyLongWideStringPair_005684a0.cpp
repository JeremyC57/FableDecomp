// _global::_Destroy_LongWideStringPair(SLongWideString* first, SLongWideString* last) -- destroy the CWideString half of every {long, CWideString} pair in [first, last)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005684A0, length 34 bytes (padded to 48).
// Leaf: _Destroy_LongWideStringPair
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is STLport
// `_Destroy(first, last)` emitted out of line (first in ecx, last in edx, no tag -> plain `ret`) over 8-byte
// pair-like elements: a scalar key at +0 and a CWideString at +4 whose destructor @ 0x0099B510 (landed) is the only
// non-trivial part of the implicit element destructor.
// Callees: CWideString::~CWideString @ 0x0099B510.
// Shape: guarded do-while, `last` in edi, `lea ecx,[esi+4]` for the second member.
class CWideString { public:
    void* m_pData;
    ~CWideString();                         // 0x0099B510
};
struct SLongWideString {                    // 8-byte element (pair<long, CWideString>)
    long first;                             // +0x00
    CWideString second;                     // +0x04
};

void __fastcall _Destroy_LongWideStringPair(SLongWideString* first, SLongWideString* last)
{
    for (; first != last; ++first)
        first->~SLongWideString();
}