// std::__adjust_heap(ENavigatorType* first, int holeIndex, int len, ENavigatorType value, less<ENavigatorType> comp) -- STLport heap sift-down of an ENavigatorType range, then the inline __push_heap of value
// Leaf: ??$__adjust_heap@PAW4ENavigatorType@@
//
// Retail 0x00672390, 124 bytes (+ int3 padding to 128), `ret 0Ch` (/Gr: first in ecx, holeIndex in edx; len, value
//   and the empty 4-byte comparator on the stack).
// Callers: 0x00672410 make_heap (twice), 0x00672330 __partial_sort, 0x00672460 sort_heap -- all over
//   CThingCreatureDef's vector<ENavigatorType> (sorted then uniqued in CThingCreatureDef::Transfer 0x00671600).
// Shape: STLport _heap.c -- `topIndex = holeIndex; secondChild = 2 * holeIndex + 2; while (secondChild < len) {
//   if (comp(first[secondChild], first[secondChild - 1])) secondChild--; first[holeIndex] = first[secondChild];
//   holeIndex = secondChild; secondChild = 2 * (secondChild + 1); } if (secondChild == len) { first[holeIndex] =
//   first[secondChild - 1]; holeIndex = secondChild - 1; } __push_heap(first, holeIndex, topIndex, value, comp);`
//   with __push_heap inline (signed `(holeIndex - 1) / 2` = `cdq ; sub ; sar`).  less<> on an enum is a signed
//   `jge`.  The retail TU is /Gr; the convention is written as an explicit __fastcall.
// Label: catalog "_Adjust_heap<ENavigatorType*,int,ENavigatorType>" is the Ego_r (Dinkumware) spelling; retail STL is
//   STLport, so the symbol is __adjust_heap.
enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& x, const _Tp& y) const { return x < y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
__forceinline void __push_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance topIndex, _Tp value,
                        _Compare comp)
{
    _Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp)
{
    _Distance topIndex = holeIndex;
    _Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}
}

template void __fastcall std::__adjust_heap<ENavigatorType*, int, ENavigatorType, std::less<ENavigatorType> >(
    ENavigatorType*, int, int, ENavigatorType, std::less<ENavigatorType>);