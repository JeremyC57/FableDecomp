// std::__copy_backward(CMapInfo* first, CMapInfo* last, CMapInfo* result, const random_access_iterator_tag&, int*) -- assign [first, last) onto the range ending at result, back to front
// Leaf: ??$__copy_backward@PAVCMapInfo@@PAV1@H@std@@YIPAVCMapInfo@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00513D40, 67 bytes, `__fastcall` (first ecx, last edx; result, the tag and the distance
//   pointer on the stack), `ret 0Ch`.
// Identity (proven): the per-element call is the landed CMapInfo::operator=(const CMapInfo&) 0x00513BF0 and the stride is
//   sizeof(CMapInfo) = 72 (the reciprocal divide and the `sub 0x48` steps); callers vector<CMapInfo>::_M_range_insert 0x00519CD0 / 0x00515020 / 0x0051A6D0. The manifest's
//   "_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*...>" is a propagated (later-STL) label.
// Callees: CMapInfo::operator=(const CMapInfo&) 0x00513BF0.
// Shape (family M108, representative 0x00513D40): STLport 4.6 `for (_Distance __n = __last - __first; __n > 0; --__n)
//   *--__result = *--__last; return __result;`.
class CMapInfo {                                             // 72 bytes in retail
public:
    CMapInfo& operator=(const CMapInfo& other);                 // 0x00513BF0
    unsigned long Members[18];
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
template ::CMapInfo* __fastcall __copy_backward<::CMapInfo*, ::CMapInfo*, int>(::CMapInfo*, ::CMapInfo*, ::CMapInfo*, const random_access_iterator_tag&, int*);
}