// _global::_Destroy_StringRecordC(SStringRecordC* first, SStringRecordC* last) -- destroy [first, last) of 12-byte records led by a CCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E5360, length 33 bytes (padded to 48).
// Leaf: _Destroy_StringRecordC
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is STLport
// `_Destroy(first, last)` emitted out of line (first in ecx, last in edx, no tag -> plain `ret`) over 0x0c-byte
// elements whose implicit destructor reduces to ~CCharString @ 0x0099EAE0 on the leading member (the other 8 bytes are
// trivially destructible). The element class is not identified (its only caller, 0x00A4A5EB, is itself propagated).
// Callees: CCharString::~CCharString @ 0x0099EAE0.
// Shape: guarded do-while, `last` in edi, the element address straight into ecx (`mov ecx,esi`).
class CCharString { public:
    void* m_pData;
    ~CCharString();                         // 0x0099EAE0
};
struct SStringRecordC {                     // 12-byte element
    CCharString m_name;                     // +0x00
    long m_values[2];                       // +0x04
};

void __fastcall _Destroy_StringRecordC(SStringRecordC* first, SStringRecordC* last)
{
    for (; first != last; ++first)
        first->~SStringRecordC();
}