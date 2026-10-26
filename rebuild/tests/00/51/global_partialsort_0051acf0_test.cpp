// Behaviour test for 0x0051ACF0 __partial_sort<CThingBuilding**, CIsHouseLargerThan>: after the call [first, middle)
// holds the (middle - first) largest houses in comparator order (largest first) and the whole range is a permutation of
// the input.  __adjust_heap, __make_heap and sort_heap are the STLport bodies.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CThingBuilding { public: long HouseSize; };
class CIsHouseLargerThan { public:
    bool operator()(const CThingBuilding* a, const CThingBuilding* b) const { return a->HouseSize > b->HouseSize; }
};
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance topIndex, _Tp value, _Compare comp)
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
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value, _Compare comp)
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*)
{
    if (last - first < 2) return;
    _Distance len = last - first;
    _Distance parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, _Tp(*(first + parent)), comp);
        if (parent == 0) return;
        parent--;
    }
}

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result,
                       _Tp value, _Compare comp, _Distance*)
{
    *result = *first;
    __adjust_heap(first, _Distance(0), _Distance(last - first), value, comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)
{
    while (last - first > 1) {
        --last;
        __pop_heap(first, last, last, *last, comp, (int*)0);
    }
}
template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)
{
    __make_heap(first, last, comp, (CThingBuilding**)0, (int*)0);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator first, _RandomAccessIterator middle, _RandomAccessIterator last,
                               _Tp*, _Compare comp)
{
    make_heap(first, middle, comp);
    for (_RandomAccessIterator i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

int main()
{
    static const long sizes[12] = { 7, 3, 9, 1, 4, 4, 12, 0, 6, 2, 8, 5 };
    CThingBuilding b[12];
    CThingBuilding* a[12];
    for (int i = 0; i < 12; ++i) b[i].HouseSize = sizes[i];
    CIsHouseLargerThan comp;
    int ok = 1;
    for (int m = 0; m <= 12; ++m) {
        for (int i = 0; i < 12; ++i) a[i] = &b[i];
        __partial_sort(a, a + m, a + 12, (CThingBuilding**)0, comp);
        // expected top-m sizes in descending order: 12 9 8 7 6 5 4 4 3 2 1 0
        static const long want[12] = { 12, 9, 8, 7, 6, 5, 4, 4, 3, 2, 1, 0 };
        for (int i = 0; i < m; ++i) ok &= a[i]->HouseSize == want[i];
        int seen[12] = { 0 };
        for (int i = 0; i < 12; ++i) { int k = (int)(a[i] - b); if (k >= 0 && k < 12) seen[k]++; else ok = 0; }
        for (int i = 0; i < 12; ++i) ok &= seen[i] == 1;
    }
    if (!ok) { std::printf("0051acf0_TEST FAIL\n"); return 1; }
    std::printf("0051acf0_TEST PASS\n");
    return 0;
}