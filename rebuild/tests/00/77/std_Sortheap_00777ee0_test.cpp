#include <cstdio>
// Behaviour test for 0x00777EE0: STLport sort_heap over 4-byte signed integers with less<>.
struct LessLong { bool operator()(const long& a, const long& b) const { return a < b; } };

// STLport heap chain (behaviour model of the retail row and its out-of-line callees)
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp)
{
    *__result = *__first;
    __adjust_heap(__first, 0, int(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, *(__last - 1), __comp);
}
template <class _RandomAccessIterator, class _Compare>
void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void make_heap_by_push(_RandomAccessIterator __first, int __n, _Compare __comp)
{
    for (int i = 1; i < __n; ++i)
        __push_heap(__first, i, 0, *(__first + i), __comp);
}

int main()
{
    long v[9] = { 5, -3, 12, 0, 7, -40, 7, 99, 1 };
    make_heap_by_push(v, 9, LessLong());
    sort_heap(v, v + 9, LessLong());
    long want[9] = { -40, -3, 0, 1, 5, 7, 7, 12, 99 };
    for (int i = 0; i < 9; ++i) if (v[i] != want[i]) { std::printf("00777ee0_TEST FAIL\n"); return 1; }
    long one[1] = { 4 };
    sort_heap(one, one + 1, LessLong());
    if (one[0] != 4) { std::printf("00777ee0_TEST FAIL\n"); return 1; }
    std::printf("00777ee0_TEST PASS\n");
    return 0;
}