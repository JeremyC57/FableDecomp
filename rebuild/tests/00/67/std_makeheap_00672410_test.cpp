// Behaviour test for 0x00672410 std::make_heap<ENavigatorType*, less<ENavigatorType> >: build a max-heap in place;
// the callee __adjust_heap (0x00672390) is modelled with its landed body.
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

static int g_adjustCalls = 0;

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator first, _Distance holeIndex, _Distance len, _Tp value,
                              _Compare comp)                                  // 0x00672390 model
{
    ++g_adjustCalls;
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

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
__forceinline void __make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp, _Tp*, _Distance*)
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

template <class _RandomAccessIterator, class _Compare>
void __fastcall make_heap(_RandomAccessIterator first, _RandomAccessIterator last, _Compare comp)
{
    __make_heap(first, last, comp, __value_type(first), __distance_type(first));
}
}

static bool IsHeap(const ENavigatorType* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[(i - 1) / 2] < a[i]) return false;
    return true;
}

int main()
{
    ENavigatorType a[9] = { (ENavigatorType)3, (ENavigatorType)-1, (ENavigatorType)7, (ENavigatorType)0,
                            (ENavigatorType)9, (ENavigatorType)4, (ENavigatorType)2, (ENavigatorType)8,
                            (ENavigatorType)5 };
    nav::make_heap(a, a + 9, nav::less<ENavigatorType>());
    int sum = 0;
    for (int i = 0; i < 9; ++i) sum += a[i];
    if (!IsHeap(a, 9) || a[0] != 9 || sum != 37 || nav::g_adjustCalls != 4) { std::printf("00672410_TEST FAIL\n"); return 1; }

    // fewer than two elements: nothing is touched, no sift call
    nav::g_adjustCalls = 0;
    ENavigatorType b[1] = { (ENavigatorType)6 };
    nav::make_heap(b, b + 1, nav::less<ENavigatorType>());
    nav::make_heap(b, b, nav::less<ENavigatorType>());
    if (b[0] != 6 || nav::g_adjustCalls != 0) { std::printf("00672410_TEST FAIL\n"); return 1; }

    // two elements: one sift of parent 0
    ENavigatorType c[2] = { (ENavigatorType)1, (ENavigatorType)2 };
    nav::make_heap(c, c + 2, nav::less<ENavigatorType>());
    if (c[0] != 2 || c[1] != 1 || nav::g_adjustCalls != 1) { std::printf("00672410_TEST FAIL\n"); return 1; }

    std::printf("00672410_TEST PASS\n");
    return 0;
}