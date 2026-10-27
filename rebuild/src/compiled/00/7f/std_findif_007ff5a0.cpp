// std::__find_if(std::pair<EClothingSuitPart, bool>* first, std::pair<EClothingSuitPart, bool>* last, CIsSuitPartType pred, const random_access_iterator_tag&) -- the first pair whose key matches the predicate's
// Leaf: ??$__find_if@PAU?$pair@W4EClothingSuitPart@@_N@std@@VCIsSuitPartType@@@std@@YIPAU?$pair@W4EClothingSuitPart@@_N@0@PAU10@0VCIsSuitPartType@@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FF5A0, 102 bytes, `__fastcall` (first ecx, last edx; the 4-byte predicate by value and
//   the tag reference on the stack), `ret 8`.
// Identity (proven): called from the landed CTCDHeroSuit::OnCreate 0x007FF8A0 (SuitParts, vector<pair<EClothingSuitPart, bool> >); Ego_d.pdb ??$_Find_if@PAU?$pair@W4EClothingSuitPart@@_N@std@@VCIsSuitPartType@@@std@@ (CIsSuitPartType: SuitPartType).
// Shape (family M127, representative 0x00547790): STLport 4.6 random-access __find_if -- `__trip_count = (last - first) >> 2`
//   x4-unrolled, then the `switch (last - first)` remainder falling through 3/2/1; the predicate's `x.first == SuitPartType` inline.
enum EClothingSuitPart { CLOTHING_SUIT_PART_NONE = 0 };
namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _RandomAccessIter, class _Predicate>
_RandomAccessIter __fastcall __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                       const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
    case 0:
    default:
        return __last;
    }
}
}
class CIsSuitPartType {                                              // Ego_r.pdb, 4 bytes
public:
    bool operator()(const std::pair<EClothingSuitPart, bool>& x) const { return x.first == SuitPartType; }
    EClothingSuitPart SuitPartType;
};
template std::pair<EClothingSuitPart, bool>* __fastcall std::__find_if<std::pair<EClothingSuitPart, bool>*, CIsSuitPartType>(std::pair<EClothingSuitPart, bool>*, std::pair<EClothingSuitPart, bool>*, CIsSuitPartType, const std::random_access_iterator_tag&);