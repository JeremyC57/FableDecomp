#include <cstdio>
// Behaviour test for 0x008182E0: STLport sort_heap over 32-byte THitLocationIntersectionElement keyed on key.
struct CHitLocation { int id; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct THitLocationIntersectionElement { long key; CHitLocation* hit_location; C3DVector intersection_pos; C3DVector outward_normal; };
struct THitLocationIntersectionElementPred {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};

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
    CHitLocation loc[6];
    THitLocationIntersectionElement v[6];
    long keys[6] = { 40, 10, 60, 20, 50, 30 };
    for (int i = 0; i < 6; ++i) {
        v[i].key = keys[i]; v[i].hit_location = &loc[i];
        v[i].intersection_pos.X = (float)keys[i]; v[i].intersection_pos.Y = 1.0f; v[i].intersection_pos.Z = 2.0f;
        v[i].outward_normal.X = 0.0f; v[i].outward_normal.Y = 0.0f; v[i].outward_normal.Z = (float)i;
    }
    make_heap_by_push(v, 6, THitLocationIntersectionElementPred());
    sort_heap(v, v + 6, THitLocationIntersectionElementPred());
    long wk[6] = { 10, 20, 30, 40, 50, 60 };
    int wi[6] = { 1, 3, 5, 0, 4, 2 };
    for (int i = 0; i < 6; ++i)
        if (v[i].key != wk[i] || v[i].hit_location != &loc[wi[i]] || v[i].intersection_pos.X != (float)wk[i] || v[i].outward_normal.Z != (float)wi[i]) {
            std::printf("008182e0_TEST FAIL\n"); return 1;
        }
    std::printf("008182e0_TEST PASS\n");
    return 0;
}