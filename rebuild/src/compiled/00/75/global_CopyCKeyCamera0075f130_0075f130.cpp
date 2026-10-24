// _global::Copy_CKeyCamera_0075f130(const CKeyCamera* first, const CKeyCamera* last, CKeyCamera* result, const random_access_iterator_tag&, int*) -- STLport __copy<CKeyCamera*>: counted forward assignment of 56-byte elements, returns the advanced result
// Leaf: Copy_CKeyCamera_0075f130
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F130, length 71 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CKeyCamera::operator= @ 0x0075EF00.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x92492493 ; add ; sar 5 (/56)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x38; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is CKeyCamera (its landed operator= @ 0x0075EF00, catalog "CEnginePrimitive2DText::operator="; the landed _Uninit_copy<CKeyCamera*> @ 0x0075F3E0 and _Copy_backward @ 0x0075F0E0 are siblings).
struct CKeyCamera {                              // 56 bytes (0x38); its own assignment copies the body
    unsigned char _body[0x38];
    CKeyCamera& operator=(const CKeyCamera& other);   // 0x0075EF00
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CKeyCamera* __fastcall Copy_CKeyCamera_0075f130(const CKeyCamera* first, const CKeyCamera* last, CKeyCamera* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}