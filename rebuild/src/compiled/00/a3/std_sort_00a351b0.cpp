// std::sort(std::pair<unsigned long, C3DDummyObject>* first, std::pair<unsigned long, C3DDummyObject>* last, CKeyPairCompareLess<unsigned long, C3DDummyObject> comp) -- introsort the range, then finish with the final insertion sort
// Leaf: ??$sort@PAU?$pair@KVC3DDummyObject@@@std@@V?$CKeyPairCompareLess@KVC3DDummyObject@@@@@std@@YIXPAU?$pair@KVC3DDummyObject@@@0@0V?$CKeyPairCompareLess@KVC3DDummyObject@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A351B0, 85 bytes, `__fastcall` (first ecx, last edx, the comparator's slot on the stack), `ret 4`.
// Identity (probable): its __final_insertion_sort 0x00A34780 calls the __insertion_sort 0x00A34150 catalogued as _Insertion_sort1<pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess<unsigned long, C3DDummyObject> > (56-byte records, the `sar 5` divide), in the TU of the landed CVectorMap<unsigned long, C3DDummyObject>::Insert 0x00A349B0; its callers are that map's Sort 0x00A35330 (landed: std::sort of the 0x38-byte records) and 0x00A2F740.
// Callees: std::__introsort_loop 0x00A34C00, std::__final_insertion_sort 0x00A34780 (both __fastcall).
// Shape (family M094, representative 0x004D2570): STLport 4.6 _algo.c `if (__first != __last) { __introsort_loop(__first, __last,
//   _STLP_VALUE_TYPE(__first), __lg(__last - __first) * 2, __comp); __final_insertion_sort(__first, __last, __comp); }` -- __lg
//   inline (`sar 1 ; inc` until 1).
class C3DDummyObject {                                    // 52 bytes
public:
    unsigned long Members[13];
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
template <class Key, class Value> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};
namespace std {
template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1) ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00A34C00
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00A34780
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (std::pair<unsigned long, C3DDummyObject>*)0, __lg(__last - __first) * 2, __comp);
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<std::pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess<unsigned long, C3DDummyObject> >(std::pair<unsigned long, C3DDummyObject>*, std::pair<unsigned long, C3DDummyObject>*, CKeyPairCompareLess<unsigned long, C3DDummyObject>);
}