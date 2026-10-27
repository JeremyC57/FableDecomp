// std::__make_heap(_E* first, _E* last, CKeyPairCompareLess<long, long> comp, _E*, int*) -- (_E = pair<long, long>) heapify [first, last)
// Leaf: ??$__make_heap@PAU?$pair@JJ@std@@V?$CKeyPairCompareLess@JJ@@U12@H@std@@YIXPAU?$pair@JJ@0@0V?$CKeyPairCompareLess@JJ@@0PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F8E12, 70 bytes, `__fastcall` (first ecx, last edx; comp, _Tp*, _Distance* on the stack), `ret 0Ch`.
// Identity (proven): its caller 0x005F93E6 is reached from the landed std::__partial_sort 0x005F9DA3 (pair<long, long>, CKeyPairCompareLess).
// Callees: std::__adjust_heap<_E*, int, _E, CKeyPairCompareLess<long, long> > 0x005F86AA (__fastcall, value and comparator by value).
// Shape (family M061, representative 0x00471530): STLport `if (__last - __first < 2) return; __len = __last - __first; __parent =
//   (__len - 2) / 2; for (;;) { __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp); if (__parent == 0)
//   return; __parent--; }` -- the pair copied into the argument area through the forced-inline pair copy; `optimize("s", on)`.
#pragma optimize("s", on)
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    __forceinline pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}
template <class Key, class Value> class CKeyPairCompareLess {   // empty; passed in a 4-byte slot
public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x005F86AA
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
typedef pair<long, long> _E;
template void __fastcall __make_heap(_E*, _E*, CKeyPairCompareLess<long, long>, _E*, int*);
}