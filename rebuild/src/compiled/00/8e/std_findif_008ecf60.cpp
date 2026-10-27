// std::__find_if(const std::pair<long, CCharString>* first, const std::pair<long, CCharString>* last, CIsItemWithDefIndex pred, const random_access_iterator_tag&) -- the first pair whose key matches the predicate's
// Leaf: ??$__find_if@PBU?$pair@JVCCharString@@@std@@VCIsItemWithDefIndex@@@std@@YIPBU?$pair@JVCCharString@@@0@PBU10@0VCIsItemWithDefIndex@@ABUrandom_access_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008ECF60, 102 bytes, `__fastcall` (first ecx, last edx; the 4-byte predicate by value and
//   the tag reference on the stack), `ret 8`.
// Identity (proven): its only caller is the landed CTCContainer::ContainsItem(long) const 0x008ED010 (ContainerItems, vector<pair<long, CCharString> >, const iterators: the const twin of the landed 0x008ECEF0); Ego_d.pdb has _Find_if<const pair<long, CCharString>*, CIsItemWithDefIndex> (CIsItemWithDefIndex: DefIndex).
// Shape (family M127, representative 0x00547790): STLport 4.6 random-access __find_if -- `__trip_count = (last - first) >> 2`
//   x4-unrolled, then the `switch (last - first)` remainder falling through 3/2/1; the predicate's `x.first == DefIndex` inline.
class CCharString { public: void* PStringData; };
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
class CIsItemWithDefIndex {                                              // Ego_r.pdb, 4 bytes
public:
    bool operator()(const std::pair<long, CCharString>& x) const { return x.first == DefIndex; }
    long DefIndex;
};
template const std::pair<long, CCharString>* __fastcall std::__find_if<const std::pair<long, CCharString>*, CIsItemWithDefIndex>(const std::pair<long, CCharString>*, const std::pair<long, CCharString>*, CIsItemWithDefIndex, const std::random_access_iterator_tag&);