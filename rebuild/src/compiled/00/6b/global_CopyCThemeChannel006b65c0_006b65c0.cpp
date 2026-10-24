// _global::Copy_CThemeChannel_006b65c0(const CThemeChannel* first, const CThemeChannel* last, CThemeChannel* result, const random_access_iterator_tag&, int*) -- STLport __copy<CThemeChannel*>: counted forward assignment of 496-byte elements, returns the advanced result
// Leaf: Copy_CThemeChannel_006b65c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B65C0, length 77 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CThemeChannel::operator= @ 0x006B6180.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x84210843 ; add ; sar 8 (/496)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x1f0; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is NEnvironmentInternal::CThemeChannel (its landed operator= @ 0x006B6180; catalog "CBlendedEnvironmentTheme::operator=").
struct CThemeChannel {                           // 496 bytes (0x1f0); its own assignment copies the body
    unsigned char _body[0x1f0];
    CThemeChannel& operator=(const CThemeChannel& other);   // 0x006B6180
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CThemeChannel* __fastcall Copy_CThemeChannel_006b65c0(const CThemeChannel* first, const CThemeChannel* last, CThemeChannel* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}