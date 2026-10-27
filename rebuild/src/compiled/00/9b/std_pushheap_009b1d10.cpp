// std::__push_heap(std::pair<unsigned long, long >* first, int holeIndex, int topIndex, std::pair<unsigned long, long > value, CKeyPairCompareLess<unsigned long, long > comp) -- STLport heap sift-up: move parents down while they compare below value, then drop value into the hole
// Leaf: ??$__push_heap@PAU?$pair@KJ@std@@HU12@V?$CKeyPairCompareLess@KJ@@@std@@YIXPAU?$pair@KJ@0@HHU10@V?$CKeyPairCompareLess@KJ@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009b1d10, 79 bytes, /Gr __fastcall (first in ecx, holeIndex in edx; topIndex, value, comp on the stack), `ret 10h`.
// Identity (proven): its only caller is the landed __adjust_heap 0x009B2FF0, under the __make_heap 0x009B3C70 bound ??$__make_heap@PAU?$pair@KJ@std@@V?$CKeyPairCompareLess@KJ@@U12@H@std@@... (and the matching sort_heap 0x009B5BB0 / __partial_sort 0x009B6B70); bound only as the stand-in ?_Push_heap@FableHeap_009b2ff0@@....
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
template void __fastcall std::__push_heap(std::pair<unsigned long, long >*, int, int, std::pair<unsigned long, long >, CKeyPairCompareLess<unsigned long, long >);