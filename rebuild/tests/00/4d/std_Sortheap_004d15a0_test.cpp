#include <cstdio>
// Behaviour test for 0x004D15A0: STLport sort_heap over pair<float,EConversationAttitude> keyed on the float.
enum EConversationAttitude { ATT_A = 1, ATT_B = 2, ATT_C = 3, ATT_D = 4, ATT_E = 5 };
struct Elem {
    float first; EConversationAttitude second;
    Elem() : first(0), second(ATT_A) { }
    Elem(const Elem& o) : first(o.first), second(o.second) { }
};
struct CKeyPairCompareLess { bool operator()(const Elem& a, const Elem& b) const { return a.first < b.first; } };

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
    Elem v[5];
    float k[5] = { 0.5f, -2.0f, 4.0f, 1.0f, 3.0f };
    EConversationAttitude a[5] = { ATT_A, ATT_B, ATT_C, ATT_D, ATT_E };
    for (int i = 0; i < 5; ++i) { v[i].first = k[i]; v[i].second = a[i]; }
    make_heap_by_push(v, 5, CKeyPairCompareLess());
    sort_heap(v, v + 5, CKeyPairCompareLess());
    float wk[5] = { -2.0f, 0.5f, 1.0f, 3.0f, 4.0f };
    EConversationAttitude wa[5] = { ATT_B, ATT_A, ATT_D, ATT_E, ATT_C };
    for (int i = 0; i < 5; ++i) if (v[i].first != wk[i] || v[i].second != wa[i]) { std::printf("004d15a0_TEST FAIL\n"); return 1; }
    std::printf("004d15a0_TEST PASS\n");
    return 0;
}