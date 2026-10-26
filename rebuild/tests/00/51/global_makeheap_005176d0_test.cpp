// Behaviour test for 0x005176D0 __make_heap<CThingBuilding**, CIsHouseLargerThan>: after the build, no parent compares
// "larger" than its child (the smallest house is at the root), the range is a permutation of the input, and ranges of
// fewer than two elements are left alone.  __adjust_heap is the STLport body.
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

int main()
{
    static const long sizes[11] = { 7, 3, 9, 1, 4, 4, 12, 0, 6, 2, 8 };
    CThingBuilding b[11];
    CThingBuilding* a[11];
    for (int i = 0; i < 11; ++i) { b[i].HouseSize = sizes[i]; a[i] = &b[i]; }
    CIsHouseLargerThan comp;
    int ok = 1;
    for (int n = 0; n <= 11; ++n) {
        for (int i = 0; i < 11; ++i) a[i] = &b[i];
        __make_heap(a, a + n, comp, (CThingBuilding**)0, (int*)0);
        if (n < 2) { for (int i = 0; i < 11; ++i) ok &= a[i] == &b[i]; continue; }
        for (int c = 1; c < n; ++c) ok &= !comp(a[(c - 1) / 2], a[c]);
        int seen[11] = { 0 };
        for (int i = 0; i < n; ++i) { int k = (int)(a[i] - b); ok &= k >= 0 && k < n; if (k >= 0 && k < 11) seen[k]++; }
        for (int i = 0; i < n; ++i) ok &= seen[i] == 1;
        for (int i = n; i < 11; ++i) ok &= a[i] == &b[i];
        if (n == 11) ok &= a[0]->HouseSize == 0;
    }
    if (!ok) { std::printf("005176d0_TEST FAIL\n"); return 1; }
    std::printf("005176d0_TEST PASS\n");
    return 0;
}