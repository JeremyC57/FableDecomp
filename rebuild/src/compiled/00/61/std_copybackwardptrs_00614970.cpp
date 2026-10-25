// std::__copy_backward_ptrs(first, last, result, const __false_type&) -- (CTCInventoryMap::CUIRegionInfo) STLport backward copy of 40-byte records: n = last - first by the reciprocal divide, then `*--result = *--last` n times (the implicit assignment is one 10-dword `rep movsd`); returns result - n
// Leaf: ??$__copy_backward_ptrs@PAVCUIRegionInfo@CTCInventoryMap@@PAV12@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00614970, 62 bytes (+ int3 padding), __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`.
// Identity: element CTCInventoryMap::CUIRegionInfo -- proven; evidence: its only caller is the landed vector<CTCInventoryMap::CUIRegionInfo>::_M_fill_insert 0x00618990 (whose overflow helper 0x00618420 also serves CTCInventoryMap::ConstructRegionDescription; Ego_r.pdb CUIRegionInfo is ten longs); the landed binding names it __copy_backward_ptrs<CTCInventoryMap::CUIRegionInfo>.
// Label: the manifest's "_Copy_backward<std::pair<unsigned_long,C3DDummyObject>*,...>" is propagated across byte-identical
//   bodies (an Ego_r Dinkumware spelling; no row of the family copies that pair).
// Body: STLport 4.6 _algobase.h: `__copy_backward_ptrs(first, last, result, const __false_type&)` = `__copy_backward(first,
//   last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` (the loop above); `__fastcall` on the template
//   definition (the TU's /Gr convention). VC7.1 keeps __copy_backward out of line under plain `inline` (a 25-byte
//   forwarder, graded), so it is `__forceinline` here.
class CTCInventoryMap {
public:
    class CUIRegionInfo {                                    // 40 bytes (Ego_r.pdb UIRegionsInfo element)
    public:
        long Region, Shops, BarberShops, TitleShops, TatooShops;
        long ShopsForSale, HousesForSale, ShopsOwned, HousesOwned, Taverns;
    };
};
namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>   // inline in every retail body
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CTCInventoryMap::CUIRegionInfo* __fastcall __copy_backward_ptrs<CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*>(CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, const __false_type&);
}