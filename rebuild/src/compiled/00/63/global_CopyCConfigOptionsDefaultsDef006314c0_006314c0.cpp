// _global::Copy_CConfigOptionsDefaultsDef_006314c0(const CConfigOptionsDefaultsDef* first, const CConfigOptionsDefaultsDef* last, CConfigOptionsDefaultsDef* result, const random_access_iterator_tag&, int*) -- STLport __copy<CConfigOptionsDefaultsDef*>: counted forward assignment of 124-byte elements, returns the advanced result
// Leaf: Copy_CConfigOptionsDefaultsDef_006314c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006314C0, length 71 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CConfigOptionsDefaultsDef::operator= @ 0x00625350.
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x84210843 ; add ; sar 6 (/124)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x7c; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is CConfigOptionsDefaultsDef (its landed operator= @ 0x00625350 is the only callee).
class CConfigOptionsDefaultsDef { public:               // 124 bytes (0x7c); its own assignment copies the body
    unsigned char _body[0x7c];
    CConfigOptionsDefaultsDef& operator=(const CConfigOptionsDefaultsDef& other);   // 0x00625350
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

CConfigOptionsDefaultsDef* __fastcall Copy_CConfigOptionsDefaultsDef_006314c0(const CConfigOptionsDefaultsDef* first, const CConfigOptionsDefaultsDef* last, CConfigOptionsDefaultsDef* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}