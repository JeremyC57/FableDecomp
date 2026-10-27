// std::__find_if(std::pair<unsigned long, float>* first, std::pair<unsigned long, float>* last, CIsStateSwapping pred, const random_access_iterator_tag&) -- the first pair whose key matches the predicate's
// Leaf: ??$__find_if@PAU?$pair@KM@std@@VCIsStateSwapping@@@std@@YIPAU?$pair@KM@0@PAU10@0VCIsStateSwapping@@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00547790, 102 bytes, `__fastcall` (first ecx, last edx; the 4-byte predicate by value and
//   the tag reference on the stack), `ret 8`.
// Identity (proven): its only caller is the landed NUISystem::CSwappingStateComponent::Update 0x00547380 (find_if over SwappingStates, vector<pair<unsigned long, float> >, Ego_r.pdb); Ego_d.pdb ??$_Find_if@PAU?$pair@KM@std@@VCIsStateSwapping@@@std@@ (CIsStateSwapping: 4 bytes, Index).
// Shape (family M127, representative 0x00547790): STLport 4.6 random-access __find_if -- `__trip_count = (last - first) >> 2`
//   x4-unrolled, then the `switch (last - first)` remainder falling through 3/2/1; the predicate's `x.first == Index` inline.
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
class CIsStateSwapping {                                              // Ego_r.pdb, 4 bytes
public:
    bool operator()(const std::pair<unsigned long, float>& x) const { return x.first == Index; }
    unsigned long Index;
};
template std::pair<unsigned long, float>* __fastcall std::__find_if<std::pair<unsigned long, float>*, CIsStateSwapping>(std::pair<unsigned long, float>*, std::pair<unsigned long, float>*, CIsStateSwapping, const std::random_access_iterator_tag&);