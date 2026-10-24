// _global::Copy_C3DMeshInfo_00a2bbb0(const C3DMeshInfo* first, const C3DMeshInfo* last, C3DMeshInfo* result, const random_access_iterator_tag&, int*) -- STLport __copy<C3DMeshInfo*>: counted forward assignment of 96-byte elements, returns the advanced result
// Leaf: Copy_C3DMeshInfo_00a2bbb0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A2BBB0, length 67 bytes (+ int3 padding), `ret 0Ch`.
// Callees: C3DMeshInfo::operator= @ 0x00A2B3C0.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x2aaaaaab ; sar 4 (/96)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x60; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is C3DMeshInfo (its landed operator= @ 0x00A2B3C0; catalog "CAnimationEntry::operator=").
struct C3DMeshInfo {                             // 96 bytes (0x60); its own assignment copies the body
    unsigned char _body[0x60];
    C3DMeshInfo& operator=(const C3DMeshInfo& other);   // 0x00A2B3C0
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

C3DMeshInfo* __fastcall Copy_C3DMeshInfo_00a2bbb0(const C3DMeshInfo* first, const C3DMeshInfo* last, C3DMeshInfo* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}