// _global::Copy_SStringPairWord0C_00a40040(const SStringPairWord0C* first, const SStringPairWord0C* last, SStringPairWord0C* result, const random_access_iterator_tag&, int*) -- STLport __copy<SStringPairWord0C*>: counted forward assignment of 12-byte elements, returns the advanced result
// Leaf: Copy_SStringPairWord0C_00a40040
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A40040, length 84 bytes, `ret 0Ch`.
// Callees: CCharString::operator= @ 0x0099EFB0 (m_first); CCharString::operator= @ 0x0099EFB0 (m_second).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x2aaaaaab ; sar 1 (/12)), then the element's implicit operator= inlined memberwise (each member with its own assignment is a `push src ; mov ecx,dst ; call`, a plain word a direct move),
// both pointers stepped by 0xc; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is a {CCharString, CCharString, word} record (both assignments are CCharString::operator= @ 0x0099EFB0).
struct CCharString {                         // 4 bytes: the string-data pointer
    void* PStringData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SStringPairWord0C {                       // 12 bytes; implicit memberwise operator=
    CCharString m_first;                    // +0x00
    CCharString m_second;                    // +0x04
    long m_value;                           // +0x08
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SStringPairWord0C* __fastcall Copy_SStringPairWord0C_00a40040(const SStringPairWord0C* first, const SStringPairWord0C* last, SStringPairWord0C* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}