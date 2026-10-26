// std::__copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*) -- (CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, int) STLport random-access backward copy of trivially-assigned records: n = last - first block copies `*--result = *--last`, return the new result
// Flags: /Gr
// Leaf: ??$__copy_backward@PAVCUIRegionInfo@CTCInventoryMap@@PAV12@H@std@@YIPAVCUIRegionInfo@CTCInventoryMap@@PAV12@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00613420, 68 bytes (+ int3 padding), __fastcall (/Gr: first ecx, last edx; result,
//   the tag reference and the null distance pointer on the stack), `ret 0Ch`.
// Identity (probable): std::__copy_backward<CTCInventoryMap::CUIRegionInfo*, CTCInventoryMap::CUIRegionInfo*, int>; evidence: unreferenced; it sits in CTCInventoryMap's object code among the 0x28-stride helpers of vector<CTCInventoryMap::CUIRegionInfo> (CTCInventoryMap::UIRegionsInfo, Ego_r.pdb, 40-byte CUIRegionInfo): the range erase 0x00616DA0 (J102) and the _M_fill_insert 0x00618990 called by its resize 0x006190C0 under CPersistContext::Transfer 0x006197D0 (CTCInventoryMap serialise); the object's other vector elements are long, CWideString, ECategory, CUIMiniMapMarker (0x1C, not trivially assigned) and CEnginePrimitiveHandle.
// Label: the manifest's "_Copy_backward<std::pair<CCharString,C3DDummyObject>*,std::pair<class_CCharString,class_C3DDummyObject>*>" is a propagated Dinkumware spelling (family J101: five backward copies of 0x24- to
//   0x34-byte records; retail has no identical-code folding).
// Callees: none (the 0x28-byte assignment is one `rep movsd` of 10 dwords).
// Shape: STLport 4.6 _algobase.h random-access __copy_backward verbatim (J078's model over a trivially-assigned element); the
//   count is a division by 0x28 (multiply by 0x66666667).
class CTCInventoryMap {                                    // partial view: only the nested type is used here
public:
    class CUIRegionInfo {                                  // 40 bytes (Ego_r.pdb; class key V from FableWin.pdb)
    public:
        long Region;                                       // +0x00
        long Shops;                                        // +0x04
        long BarberShops;                                  // +0x08
        long TitleShops;                                   // +0x0C
        long TatooShops;                                   // +0x10
        long ShopsForSale;                                 // +0x14
        long HousesForSale;                                // +0x18
        long ShopsOwned;                                   // +0x1C
        long HousesOwned;                                  // +0x20
        long Taverns;                                      // +0x24
    };
};
namespace std {
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
typedef CTCInventoryMap::CUIRegionInfo TElement_00613420;
template TElement_00613420* __copy_backward<TElement_00613420*, TElement_00613420*, int>(TElement_00613420*, TElement_00613420*, TElement_00613420*,
                                                                         const random_access_iterator_tag&, int*);
}