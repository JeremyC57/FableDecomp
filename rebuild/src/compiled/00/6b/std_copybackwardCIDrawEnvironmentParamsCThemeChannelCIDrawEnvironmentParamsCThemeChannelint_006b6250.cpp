// _global::CopyBackward_CElement1F0_006b6180_006b6250(CElement1F0_006b6180* first, CElement1F0_006b6180* last, CElement1F0_006b6180* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 0x1F0-byte elements with an out-of-line operator=: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_CElement1F0_006b6180_006b6250
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B6250, length 88 bytes (+ int3 padding), `ret 0Ch`.
// Callee: the element operator= @ 0x006B6180 (catalog "CBlendedEnvironmentTheme::operator="; thiscall, source pushed).
// Shape: the landed 0x00517430 / 0x0086E770 family: SGI-STL `__copy_backward(first, last, result,
// random_access_iterator_tag, Distance*)` -- `for (n = last - first; n > 0; --n) *--result = *--last;`, the moved-back
// result returned. /Gr TU (__fastcall: first in ecx, last in edx, result + the two tags on the stack). The element count
// is the reciprocal-multiply division of the byte distance by 0x1F0.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated;
// the element is modelled by its size and assignment operator.
struct CElement1F0_006b6180 {
    unsigned char _body[0x1F0];
    CElement1F0_006b6180& operator=(const CElement1F0_006b6180& other);   // 0x006B6180
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement1F0_006b6180* __fastcall CopyBackward_CElement1F0_006b6180_006b6250(CElement1F0_006b6180* first, CElement1F0_006b6180* last, CElement1F0_006b6180* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}