// _global::CopyBackward_CElementB8_00ad7050_00ad7120(CElementB8_00ad7050* first, CElementB8_00ad7050* last, CElementB8_00ad7050* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 0xB8-byte elements with an out-of-line operator=: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_CElementB8_00ad7050_00ad7120
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD7120, length 88 bytes, `ret 0Ch`.
// Callee: the element operator= @ 0x00AD7050 (catalog "COptimisedVertex" (the vertex assignment); thiscall, source pushed).
// Shape: the landed 0x00517430 / 0x0086E770 family: SGI-STL `__copy_backward(first, last, result,
// random_access_iterator_tag, Distance*)` -- `for (n = last - first; n > 0; --n) *--result = *--last;`, the moved-back
// result returned. /Gr TU (__fastcall: first in ecx, last in edx, result + the two tags on the stack). The element count
// is the reciprocal-multiply division of the byte distance by 0xB8.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated;
// the element is modelled by its size and assignment operator.
struct CElementB8_00ad7050 {
    unsigned char _body[0xB8];
    CElementB8_00ad7050& operator=(const CElementB8_00ad7050& other);   // 0x00AD7050
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElementB8_00ad7050* __fastcall CopyBackward_CElementB8_00ad7050_00ad7120(CElementB8_00ad7050* first, CElementB8_00ad7050* last, CElementB8_00ad7050* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}