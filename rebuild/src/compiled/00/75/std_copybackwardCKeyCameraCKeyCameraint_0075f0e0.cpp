// _global::CopyBackward_CElement38_0075ef00_0075f0e0(CElement38_0075ef00* first, CElement38_0075ef00* last, CElement38_0075ef00* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 0x38-byte elements with an out-of-line operator=: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_CElement38_0075ef00_0075f0e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F0E0, length 72 bytes (+ int3 padding), `ret 0Ch`.
// Callee: the element operator= @ 0x0075EF00 (catalog "CEnginePrimitive2DText::operator="; thiscall, source pushed).
// Shape: the landed 0x00517430 / 0x0086E770 family: SGI-STL `__copy_backward(first, last, result,
// random_access_iterator_tag, Distance*)` -- `for (n = last - first; n > 0; --n) *--result = *--last;`, the moved-back
// result returned. /Gr TU (__fastcall: first in ecx, last in edx, result + the two tags on the stack). The element count
// is the reciprocal-multiply division of the byte distance by 0x38.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated;
// the element is modelled by its size and assignment operator.
struct CElement38_0075ef00 {
    unsigned char _body[0x38];
    CElement38_0075ef00& operator=(const CElement38_0075ef00& other);   // 0x0075EF00
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement38_0075ef00* __fastcall CopyBackward_CElement38_0075ef00_0075f0e0(CElement38_0075ef00* first, CElement38_0075ef00* last, CElement38_0075ef00* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}