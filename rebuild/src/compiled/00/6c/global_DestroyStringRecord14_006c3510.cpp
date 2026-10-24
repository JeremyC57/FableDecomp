// _global::_Destroy_StringRecord14(SStringRecord14* first, SStringRecord14* last) -- destroy [first, last) of 20-byte records led by a CCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C3510, length 33 bytes (padded to 48).
// Leaf: _Destroy_StringRecord14
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is STLport
// `_Destroy(first, last)` emitted out of line (first in ecx, last in edx, no tag -> plain `ret`) over 0x14-byte
// elements whose implicit destructor reduces to ~CCharString @ 0x0099EAE0 on the leading member (the other 16 bytes are
// trivially destructible). The element class is not identified (its only caller, 0x00A4685E, is itself propagated).
// Callees: CCharString::~CCharString @ 0x0099EAE0.
// Shape: guarded do-while, `last` in edi, the element address straight into ecx (`mov ecx,esi`).
struct CCharString {
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
struct SStringRecord14 {                    // 20-byte element
    CCharString m_name;                     // +0x00
    long m_values[4];                       // +0x04
};

void __fastcall _Destroy_StringRecord14(SStringRecord14* first, SStringRecord14* last)
{
    for (; first != last; ++first)
        first->~SStringRecord14();
}