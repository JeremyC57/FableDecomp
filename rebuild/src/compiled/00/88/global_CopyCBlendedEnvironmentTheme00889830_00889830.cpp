// _global::Copy_CBlendedEnvironmentTheme_00889830(const CBlendedEnvironmentTheme* first, const CBlendedEnvironmentTheme* last, CBlendedEnvironmentTheme* result, const random_access_iterator_tag&, int*) -- STLport __copy<CBlendedEnvironmentTheme*>: counted forward assignment of 500-byte elements, returns the advanced result
// Leaf: Copy_CBlendedEnvironmentTheme_00889830
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00889830, length 73 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CBlendedEnvironmentTheme::operator= @ 0x006B5DF0.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x10624dd3 ; sar 5 (/500)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x1f4; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is NEnvironmentInternal::CBlendedEnvironmentTheme (its landed operator= @ 0x006B5DF0).
struct CBlendedEnvironmentTheme {                // 500 bytes (0x1f4); its own assignment copies the body
    unsigned char _body[0x1f4];
    CBlendedEnvironmentTheme& operator=(const CBlendedEnvironmentTheme& other);   // 0x006B5DF0
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CBlendedEnvironmentTheme* __fastcall Copy_CBlendedEnvironmentTheme_00889830(const CBlendedEnvironmentTheme* first, const CBlendedEnvironmentTheme* last, CBlendedEnvironmentTheme* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}