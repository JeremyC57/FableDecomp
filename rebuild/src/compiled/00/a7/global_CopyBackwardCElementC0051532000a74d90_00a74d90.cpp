// _global::CopyBackward_CElementC_00515320_00a74d90(CElementC_00515320* first, CElementC_00515320* last, CElementC_00515320* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 0xC-byte elements with an out-of-line operator=: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_CElementC_00515320_00a74d90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A74D90, length 66 bytes, `ret 0Ch`.
// Callee: the element operator= @ 0x00515320 (catalog "std::vector<C2DLineF>::operator=" (a 12-byte vector assignment); thiscall, source pushed).
// Shape: the landed 0x00517430 / 0x0086E770 family: SGI-STL `__copy_backward(first, last, result,
// random_access_iterator_tag, Distance*)` -- `for (n = last - first; n > 0; --n) *--result = *--last;`, the moved-back
// result returned. /Gr TU (__fastcall: first in ecx, last in edx, result + the two tags on the stack). The element count
// is the reciprocal-multiply division of the byte distance by 0xC.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated;
// the element is modelled by its size and assignment operator.
struct CElementC_00515320 {
    unsigned char _body[0xC];
    CElementC_00515320& operator=(const CElementC_00515320& other);   // 0x00515320
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElementC_00515320* __fastcall CopyBackward_CElementC_00515320_00a74d90(CElementC_00515320* first, CElementC_00515320* last, CElementC_00515320* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}