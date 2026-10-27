// std::__copy_backward(CRegion* first, CRegion* last, CRegion* result, const random_access_iterator_tag&, int*) -- assign [first, last) onto the range ending at result, back to front
// Leaf: ??$__copy_backward@PAVCRegion@@PAV1@H@std@@YIPAVCRegion@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00519C80, 67 bytes, `__fastcall` (first ecx, last edx; result, the tag and the distance
//   pointer on the stack), `ret 0Ch`.
// Identity (proven): the per-element call is the landed CRegion::operator=(const CRegion&) 0x00519B80 and the stride is
//   sizeof(CRegion) = 88 (the reciprocal divide and the `sub 0x58` steps); callers 0x0051A900 / 0x0051D0A0 (vector<CRegion> insertion). The manifest's
//   "_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*...>" is a propagated (later-STL) label.
// Callees: CRegion::operator=(const CRegion&) 0x00519B80.
// Shape (family M108, representative 0x00513D40): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;`.
class CRegion {                                             // 88 bytes in retail
public:
    CRegion& operator=(const CRegion& other);                 // 0x00519B80
    unsigned long Members[22];
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template ::CRegion* __fastcall __copy_backward<::CRegion*, ::CRegion*, int>(::CRegion*, ::CRegion*, ::CRegion*, const random_access_iterator_tag&, int*);
}