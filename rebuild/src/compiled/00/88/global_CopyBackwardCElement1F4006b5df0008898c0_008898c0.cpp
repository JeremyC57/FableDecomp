// _global::CopyBackward_CElement1F4_006b5df0_008898c0(CElement1F4_006b5df0* first, CElement1F4_006b5df0* last, CElement1F4_006b5df0* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 0x1F4-byte elements with an out-of-line operator=: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_CElement1F4_006b5df0_008898c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008898C0, length 73 bytes, `ret 0Ch`.
// Callee: the element operator= @ 0x006B5DF0 (catalog "CBlendedEnvironmentTheme::operator="; thiscall, source pushed).
// Shape: the landed 0x00517430 / 0x0086E770 family: SGI-STL `__copy_backward(first, last, result,
// random_access_iterator_tag, Distance*)` -- `for (n = last - first; n > 0; --n) *--result = *--last;`, the moved-back
// result returned. /Gr TU (__fastcall: first in ecx, last in edx, result + the two tags on the stack). The element count
// is the reciprocal-multiply division of the byte distance by 0x1F4.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated;
// the element is modelled by its size and assignment operator.
struct CElement1F4_006b5df0 {
    unsigned char _body[0x1F4];
    CElement1F4_006b5df0& operator=(const CElement1F4_006b5df0& other);   // 0x006B5DF0
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement1F4_006b5df0* __fastcall CopyBackward_CElement1F4_006b5df0_008898c0(CElement1F4_006b5df0* first, CElement1F4_006b5df0* last, CElement1F4_006b5df0* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}