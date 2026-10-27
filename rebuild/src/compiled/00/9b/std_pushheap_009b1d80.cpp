// std::__push_heap(std::pair<unsigned long, unsigned long >* first, int holeIndex, int topIndex, std::pair<unsigned long, unsigned long > value, CKeyPairCompareLess<unsigned long, unsigned long > comp) -- STLport heap sift-up: move parents down while they compare below value, then drop value into the hole
// Leaf: ??$__push_heap@PAU?$pair@KK@std@@HU12@V?$CKeyPairCompareLess@KK@@@std@@YIXPAU?$pair@KK@0@HHU10@V?$CKeyPairCompareLess@KK@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009b1d80, 79 bytes, /Gr __fastcall (first in ecx, holeIndex in edx; topIndex, value, comp on the stack), `ret 10h`.
// Identity (proven): its only caller is the landed __adjust_heap 0x009B3060, under the __make_heap 0x009B3CF0 bound ??$__make_heap@PAU?$pair@KK@std@@V?$CKeyPairCompareLess@KK@@U12@H@std@@... (and sort_heap 0x009B5C10 / __partial_sort 0x009B6BE0); bound only as the stand-in ?_Push_heap@FableHeap_009b3060@@....
// Callees: none (the comparator is inline: an unsigned compare of the first dword).
// Shape (family L053): STLport 4.6 _heap.c __push_heap verbatim -- parent = (hole - 1) / 2 (`cdq ; sub ; sar`), the
//   8-byte element moved as two dwords, value's key kept in ebx.
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
}
template void __fastcall std::__push_heap(std::pair<unsigned long, unsigned long >*, int, int, std::pair<unsigned long, unsigned long >, CKeyPairCompareLess<unsigned long, unsigned long >);