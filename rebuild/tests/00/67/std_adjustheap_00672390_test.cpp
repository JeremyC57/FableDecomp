// Behaviour test for 0x00672390 std::__adjust_heap<ENavigatorType*, int, ENavigatorType, less<ENavigatorType> >:
// sift a hole at the top of a max-heap down and push the value back in; the result must be a valid max-heap that
// holds the value in place of the old top.
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

static bool IsHeap(const ENavigatorType* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[(i - 1) / 2] < a[i]) return false;
    return true;
}

int main()
{
    // max-heap of 7 values; replace the top (9) by 1 and sift
    ENavigatorType h[7] = { (ENavigatorType)9, (ENavigatorType)7, (ENavigatorType)8, (ENavigatorType)3,
                            (ENavigatorType)5, (ENavigatorType)6, (ENavigatorType)2 };
    nav::__adjust_heap(h, 0, 7, (ENavigatorType)1, nav::less<ENavigatorType>());
    int sum = 0;
    for (int i = 0; i < 7; ++i) sum += h[i];
    if (!IsHeap(h, 7) || h[0] != 8 || sum != 32) { std::printf("00672390_TEST FAIL\n"); return 1; }

    // even length: the lone-child step (secondChild == len) is taken, then the value bubbles up to the top
    ENavigatorType g[4] = { (ENavigatorType)0, (ENavigatorType)4, (ENavigatorType)3, (ENavigatorType)2 };
    nav::__adjust_heap(g, 0, 4, (ENavigatorType)-3, nav::less<ENavigatorType>());
    if (g[0] != 4 || g[1] != 2 || g[2] != 3 || g[3] != -3) { std::printf("00672390_TEST FAIL\n"); return 1; }
    ENavigatorType u[4] = { (ENavigatorType)0, (ENavigatorType)4, (ENavigatorType)3, (ENavigatorType)2 };
    nav::__adjust_heap(u, 0, 4, (ENavigatorType)5, nav::less<ENavigatorType>());
    if (u[0] != 5 || u[1] != 4 || u[2] != 3 || u[3] != 2) { std::printf("00672390_TEST FAIL\n"); return 1; }

    // hole below the top: value larger than its parent stops at topIndex
    ENavigatorType k[5] = { (ENavigatorType)10, (ENavigatorType)0, (ENavigatorType)9, (ENavigatorType)4,
                            (ENavigatorType)3 };
    nav::__adjust_heap(k, 1, 5, (ENavigatorType)6, nav::less<ENavigatorType>());
    if (!IsHeap(k, 5) || k[1] != 6 || k[3] != 4 || k[4] != 3) { std::printf("00672390_TEST FAIL\n"); return 1; }

    std::printf("00672390_TEST PASS\n");
    return 0;
}