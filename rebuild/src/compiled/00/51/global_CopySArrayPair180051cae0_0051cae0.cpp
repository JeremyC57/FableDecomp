// _global::Copy_SArrayPair18_0051cae0(const SArrayPair18_0051cae0* first, const SArrayPair18_0051cae0* last, SArrayPair18_0051cae0* result, const random_access_iterator_tag&, int*) -- STLport __copy<SArrayPair18_0051cae0*>: counted forward assignment of 24-byte elements, returns the advanced result
// Leaf: Copy_SArrayPair18_0051cae0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051CAE0, length 79 bytes, `ret 0Ch`.
// Callees: SArray0C_00515320::operator= @ 0x00515320 (m_first); SArray0C_0051bcd0::operator= @ 0x0051BCD0 (m_second).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x2aaaaaab ; sar 2 (/24)), then the element's implicit operator= inlined memberwise (each member with its own assignment is a `push src ; mov ecx,dst ; call`, a plain word a direct move),
// both pointers stepped by 0x18; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is a pair of 12-byte vectors (0x00515320 is the landed std::vector<C2DLineF>::operator=, 0x0051BCD0 another vector assignment).
struct SArray0C_00515320 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_00515320& operator=(const SArray0C_00515320& other);   // 0x00515320
};
struct SArray0C_0051bcd0 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_0051bcd0& operator=(const SArray0C_0051bcd0& other);   // 0x0051BCD0
};
struct SArrayPair18_0051cae0 {                   // 24 bytes; implicit memberwise operator=
    SArray0C_00515320 m_first;        // +0x00
    SArray0C_0051bcd0 m_second;        // +0x0c
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SArrayPair18_0051cae0* __fastcall Copy_SArrayPair18_0051cae0(const SArrayPair18_0051cae0* first, const SArrayPair18_0051cae0* last, SArrayPair18_0051cae0* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}