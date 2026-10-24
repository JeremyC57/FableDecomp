// Behaviour test for 0x00672330 std::__partial_sort<ENavigatorType*, ENavigatorType, less<ENavigatorType> >: after the
// call [first, middle) holds the smallest (middle - first) values in ascending order and the rest are a permutation
// of the remainder.  make_heap / __adjust_heap / sort_heap are modelled with the STLport bodies.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif

enum ENavigatorType { NAVIGATOR_TYPE_NONE = -1, NAVIGATOR_TYPE_0 = 0 };

namespace nav {
template <class _Tp> struct less {
    bool operator()(const _Tp& x, const _Tp& y) const { return x < y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp)                                  // 0x00672390 model
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
    _Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)   // 0x00672410 model
{
    if (last - first < 2) return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, *(first + parent), comp);
        if (parent == 0) return;
        parent--;
    }
}

template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)   // 0x00672460 model
{
    while (last - first > 1) {
        --last;
        ENavigatorType value = *last;
        *last = *first;
        __adjust_heap(first, 0, (int)(last - first), value, comp);
    }
}

template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator first, _RandomAccessIterator last, _RandomAccessIterator result,
                       _Tp value, _Compare comp, _Distance*)
{
    *result = *first;
    __adjust_heap(first, _Distance(0), _Distance(last - first), value, comp);
}

template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator first, _RandomAccessIterator middle, _RandomAccessIterator last,
                               _Tp*, _Compare comp)
{
    make_heap(first, middle, comp);
    for (_RandomAccessIterator i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, _Tp(*i), comp, __distance_type(first));
    sort_heap(first, middle, comp);
}
}

int main()
{
    ENavigatorType a[10] = { (ENavigatorType)8, (ENavigatorType)3, (ENavigatorType)-2, (ENavigatorType)9,
                             (ENavigatorType)1, (ENavigatorType)7, (ENavigatorType)0, (ENavigatorType)5,
                             (ENavigatorType)4, (ENavigatorType)6 };
    nav::__partial_sort(a, a + 4, a + 10, (ENavigatorType*)0, nav::less<ENavigatorType>());
    if (a[0] != -2 || a[1] != 0 || a[2] != 1 || a[3] != 3) { std::printf("00672330_TEST FAIL\n"); return 1; }
    int sum = 0;
    for (int i = 4; i < 10; ++i) { sum += a[i]; if (a[i] < 3) { std::printf("00672330_TEST FAIL\n"); return 1; } }
    if (sum != 39) { std::printf("00672330_TEST FAIL\n"); return 1; }

    // middle == last: a plain heap sort of the whole range
    ENavigatorType b[5] = { (ENavigatorType)4, (ENavigatorType)4, (ENavigatorType)-7, (ENavigatorType)2,
                            (ENavigatorType)0 };
    nav::__partial_sort(b, b + 5, b + 5, (ENavigatorType*)0, nav::less<ENavigatorType>());
    if (b[0] != -7 || b[1] != 0 || b[2] != 2 || b[3] != 4 || b[4] != 4) { std::printf("00672330_TEST FAIL\n"); return 1; }

    std::printf("00672330_TEST PASS\n");
    return 0;
}