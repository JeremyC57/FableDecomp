// Behaviour test for 0x004C59B0: STLport __introsort_loop over NGraphicAppearance::CMeshInfo with CSortPrimaryMeshesFirst.
// The comparator reads only its left operand (not a strict weak order), so the expected element orders come
// from an independent Python port of the STLport algorithms; meshes keep their Index/SizeX/PrimaryMesh together.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

namespace NGraphicAppearance {
class CMeshInfo {
public:
    long          Index;
    float         SizeX;
    unsigned char PrimaryMesh : 1;
};
}
using NGraphicAppearance::CMeshInfo;
class CSortPrimaryMeshesFirst {
public:
    bool operator()(const CMeshInfo& a, const CMeshInfo&) const { return a.PrimaryMesh; }
};
typedef CSortPrimaryMeshesFirst TLess;

typedef CMeshInfo TElem;
static const int kBits12[12] = { 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0 };
static const int kBits20[20] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0 };
static void Fill(CMeshInfo* a, const int* bits, int n, int invert)
{
    for (int i = 0; i < n; ++i) {
        a[i].Index = i;
        a[i].SizeX = (float)i * 1.5f;
        a[i].PrimaryMesh = (unsigned char)(invert ? 1 - bits[i] : bits[i]);
    }
}
static bool Same(const CMeshInfo* a, const int* bits, int invert, const int* want, int n)
{
    for (int i = 0; i < n; ++i) {
        int t = want[i];
        float d = a[i].SizeX - (float)t * 1.5f;
        int bit = invert ? 1 - bits[t] : bits[t];
        if (a[i].Index != t || (int)a[i].PrimaryMesh != bit || d < -0.001f || d > 0.001f)
            return false;
    }
    return true;
}

// STLport 4.x sort / heap algorithms (the model's bodies and the test's copies of their out-of-line callees).
namespace _STL {
template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __push_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}

template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
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

template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    while (__last - __first > 1) {
        --__last;
        __pop_heap(__first, __last, __last, *__last, __comp, __distance_type(__first));
    }
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template <class _RandomAccessIter, class _Compare>
inline void partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, __value_type(__first), __comp);
}

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __a, _ForwardIter2 __b)
{
    __iter_swap(__a, __b, __value_type(__a));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > 16) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result)
{
    for (int __n = (int)(__last - __first); __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}

template <class _RandomAccessIter, class _Compare>
void __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last)
        return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
}

static bool TestFunction()
{
    static const int want0[20] = { 10, 7, 11, 4, 16, 18, 3, 13, 1, 15, 8, 17, 9, 19, 5, 12, 0, 2, 6, 14 };
    static const int want8[20] = { 19, 1, 18, 3, 4, 17, 16, 7, 15, 14, 10, 11, 13, 12, 9, 8, 6, 5, 2, 0 };
    static const int ident[16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    CMeshInfo a[20];
    Fill(a, kBits20, 20, 0);
    _STL::__introsort_loop(a, a + 20, (CMeshInfo*)0, 0, TLess());   // depth 0: the heap-sort fallback
    bool ok = Same(a, kBits20, 0, want0, 20);
    Fill(a, kBits20, 20, 0);
    _STL::__introsort_loop(a, a + 20, (CMeshInfo*)0, 8, TLess());
    ok = ok && Same(a, kBits20, 0, want8, 20);
    _STL::__insertion_sort(a, a + 20, TLess());         // the final pass: every primary ahead of every other mesh
    for (int i = 0; ok && i < 20; ++i)
        if ((int)a[i].PrimaryMesh != (i < 9 ? 1 : 0))
            ok = false;
    Fill(a, kBits20, 16, 0);
    _STL::__introsort_loop(a, a + 16, (CMeshInfo*)0, 8, TLess());   // 16 or fewer: untouched
    return ok && Same(a, kBits20, 0, ident, 16);
}

int main()
{
    if (!TestFunction()) {
        std::printf("004c59b0_TEST FAIL\n");
        return 1;
    }
    std::printf("004c59b0_TEST PASS\n");
    return 0;
}