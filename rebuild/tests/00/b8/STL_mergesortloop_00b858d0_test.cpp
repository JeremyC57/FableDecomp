#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#endif


class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {
    public:
        unsigned long SortValue;
        CEngineSubPrimitiveBase* SubPrimitive;
        CSortElement() {}
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

// STLport 4.6 sort / stable_sort machinery over CSortElement (the model the chunk-428 sources compile).
namespace std {
template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __t = __a; __a = __b; __b = __t; }
template <class _Tp> inline const _Tp& (min)(const _Tp& __a, const _Tp& __b) { return __b < __a ? __b : __a; }

template <class _InputIter, class _OutputIter>
_OutputIter copy(_InputIter __first, _InputIter __last, _OutputIter __result)
{
    for (int __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first; ++__result; }
    return __result;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    for (int __n = __last - __first; __n > 0; --__n) *--__result = *--__last;
    return __result;
}
template <class _ForwardIter1, class _ForwardIter2>
_ForwardIter2 __fastcall swap_ranges(_ForwardIter1 __first1, _ForwardIter1 __last1, _ForwardIter2 __first2)
{
    for ( ; __first1 != __last1; ++__first1, ++__first2)
        swap(*__first1, *__first2);
    return __first2;
}

// heap
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
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
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
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val,
                _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1) {
        --__last;
        __pop_heap(__first, __last, __last, *__last, __comp, (int*)0);
    }
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    __make_heap(__first, __middle, __comp, (_Tp*)0, (int*)0);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}

// binary search
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = __last - __first;
    while (__len > 0) {
        _Distance __half = __len >> 1;
        _ForwardIter __middle = __first + __half;
        if (__comp(*__middle, __val)) { __first = __middle; ++__first; __len = __len - __half - 1; }
        else __len = __half;
    }
    return __first;
}
template <class _ForwardIter, class _Tp, class _Compare, class _Distance>
_ForwardIter __fastcall __upper_bound(_ForwardIter __first, _ForwardIter __last, const _Tp& __val, _Compare __comp, _Distance*)
{
    _Distance __len = __last - __first;
    while (__len > 0) {
        _Distance __half = __len >> 1;
        _ForwardIter __middle = __first + __half;
        if (__comp(__val, *__middle)) __len = __half;
        else { __first = __middle; ++__first; __len = __len - __half - 1; }
    }
    return __first;
}

// insertion sort
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; }
    *__last = __val;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    _Tp __val = *__last;
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    } else
        __unguarded_linear_insert(__last, __val, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, (CSortElement*)0, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template <class _RandomAccessIter, class _Distance, class _Compare>
void __chunk_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Distance __chunk_size, _Compare __comp)
{
    while (__last - __first >= __chunk_size) {
        __insertion_sort(__first, __first + __chunk_size, __comp);
        __first += __chunk_size;
    }
    __insertion_sort(__first, __last, __comp);
}

// merging
template <class _InputIter1, class _InputIter2, class _OutputIter, class _Compare>
_OutputIter __fastcall merge(_InputIter1 __first1, _InputIter1 __last1, _InputIter2 __first2, _InputIter2 __last2,
                             _OutputIter __result, _Compare __comp)
{
    while (__first1 != __last1 && __first2 != __last2) {
        if (__comp(*__first2, *__first1)) { *__result = *__first2; ++__first2; }
        else { *__result = *__first1; ++__first1; }
        ++__result;
    }
    return copy(__first2, __last2, copy(__first1, __last1, __result));
}
template <class _RandomAccessIter1, class _RandomAccessIter2, class _Distance, class _Compare>
void __fastcall __merge_sort_loop(_RandomAccessIter1 __first, _RandomAccessIter1 __last, _RandomAccessIter2 __result,
                                  _Distance __step_size, _Compare __comp)
{
    _Distance __two_step = 2 * __step_size;
    while (__last - __first >= __two_step) {
        __result = merge(__first, __first + __step_size, __first + __step_size, __first + __two_step, __result, __comp);
        __first += __two_step;
    }
    __step_size = (min)(_Distance(__last - __first), __step_size);
    merge(__first, __first + __step_size, __first + __step_size, __last, __result, __comp);
}
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __merge_sort_with_buffer(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                         _Distance*, _Compare __comp)
{
    _Distance __len = __last - __first;
    _Pointer __buffer_last = __buffer + __len;
    _Distance __step_size = 7;                          // __stl_chunk_size
    __chunk_insertion_sort(__first, __last, __step_size, __comp);
    while (__step_size < __len) {
        __merge_sort_loop(__first, __last, __buffer, __step_size, __comp);
        __step_size *= 2;
        __merge_sort_loop(__buffer, __buffer_last, __first, __step_size, __comp);
        __step_size *= 2;
    }
}
template <class _BidirectionalIter1, class _BidirectionalIter2, class _BidirectionalIter3, class _Compare>
_BidirectionalIter3 __fastcall __merge_backward(_BidirectionalIter1 __first1, _BidirectionalIter1 __last1,
                                                _BidirectionalIter2 __first2, _BidirectionalIter2 __last2,
                                                _BidirectionalIter3 __result, _Compare __comp)
{
    if (__first1 == __last1) return copy_backward(__first2, __last2, __result);
    if (__first2 == __last2) return copy_backward(__first1, __last1, __result);
    --__last1;
    --__last2;
    for (;;) {
        if (__comp(*__last2, *__last1)) {
            *--__result = *__last1;
            if (__first1 == __last1) return copy_backward(__first2, ++__last2, __result);
            --__last1;
        } else {
            *--__result = *__last2;
            if (__first2 == __last2) return copy_backward(__first1, ++__last1, __result);
            --__last2;
        }
    }
}
// rotate by three reversals (the callees only need a correct rotate)
template <class _Iter> void __reverse(_Iter __first, _Iter __last)
{
    while (__first < __last) { --__last; swap(*__first, *__last); ++__first; }
}
template <class _Iter> _Iter rotate(_Iter __first, _Iter __middle, _Iter __last)
{
    __reverse(__first, __middle); __reverse(__middle, __last); __reverse(__first, __last);
    return __first + (__last - __middle);
}
template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp)
{
    if (__len1 == 0 || __len2 == 0) return;
    if (__len1 + __len2 == 2) {
        if (__comp(*__middle, *__first)) swap(*__first, *__middle);
        return;
    }
    _BidirectionalIter __first_cut = __first;
    _BidirectionalIter __second_cut = __middle;
    _Distance __len11 = 0;
    _Distance __len22 = 0;
    if (__len1 > __len2) {
        __len11 = __len1 / 2;
        __first_cut += __len11;
        __second_cut = __lower_bound(__middle, __last, *__first_cut, __comp, (int*)0);
        __len22 = __second_cut - __middle;
    } else {
        __len22 = __len2 / 2;
        __second_cut += __len22;
        __first_cut = __upper_bound(__first, __middle, *__second_cut, __comp, (int*)0);
        __len11 = __first_cut - __first;
    }
    _BidirectionalIter __new_middle = rotate(__first_cut, __middle, __second_cut);
    __merge_without_buffer(__first, __first_cut, __new_middle, __len11, __len22, __comp);
    __merge_without_buffer(__new_middle, __second_cut, __last, __len1 - __len11, __len2 - __len22, __comp);
}
// merge_adaptive: the test copy merges through the buffer when the shorter run fits, else without it
template <class _BidirectionalIter, class _Distance, class _Pointer, class _Compare>
void __fastcall __merge_adaptive(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                 _Distance __len1, _Distance __len2, _Pointer __buffer, _Distance __buffer_size, _Compare __comp)
{
    if (__len1 <= __len2 && __len1 <= __buffer_size) {
        _Pointer __end_buffer = copy(__first, __middle, __buffer);
        merge(__buffer, __end_buffer, __middle, __last, __first, __comp);
    } else if (__len2 <= __buffer_size) {
        _Pointer __end_buffer = copy(__middle, __last, __buffer);
        __merge_backward(__first, __middle, __buffer, __end_buffer, __last, __comp);
    } else
        __merge_without_buffer(__first, __middle, __last, __len1, __len2, __comp);
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __inplace_stable_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first < 15) {
        __insertion_sort(__first, __last, __comp);
        return;
    }
    _RandomAccessIter __middle = __first + (__last - __first) / 2;
    __inplace_stable_sort(__first, __middle, __comp);
    __inplace_stable_sort(__middle, __last, __comp);
    __merge_without_buffer(__first, __middle, __last, __middle - __first, __last - __middle, __comp);
}
template <class _RandomAccessIter, class _Pointer, class _Distance, class _Compare>
void __fastcall __stable_sort_adaptive(_RandomAccessIter __first, _RandomAccessIter __last, _Pointer __buffer,
                                       _Distance __buffer_size, _Compare __comp)
{
    _Distance __len = (__last - __first + 1) / 2;
    _RandomAccessIter __middle = __first + __len;
    if (__len > __buffer_size) {
        __stable_sort_adaptive(__first, __middle, __buffer, __buffer_size, __comp);
        __stable_sort_adaptive(__middle, __last, __buffer, __buffer_size, __comp);
    } else {
        __merge_sort_with_buffer(__first, __middle, __buffer, (_Distance*)0, __comp);
        __merge_sort_with_buffer(__middle, __last, __buffer, (_Distance*)0, __comp);
    }
    __merge_adaptive(__first, __middle, __last, _Distance(__middle - __first), _Distance(__last - __middle), __buffer,
                     __buffer_size, __comp);
}

template <class _ForwardIterator, class _Tp>
class _Temporary_buffer {
public:
    int _M_original_len;
    int _M_len;
    _Tp* _M_buffer;
    void _M_allocate_buffer()
    {
        _M_original_len = _M_len;
        _M_buffer = 0;
        if (_M_len > (int)(0x7FFFFFFF / sizeof(_Tp)))
            _M_len = 0x7FFFFFFF / sizeof(_Tp);
        while (_M_len > 0) {
            _M_buffer = (_Tp*)malloc(_M_len * sizeof(_Tp));
            if (_M_buffer)
                break;
            _M_len /= 2;
        }
    }
    int size() const { return _M_len; }
    _Tp* begin() { return _M_buffer; }
    _Temporary_buffer(_ForwardIterator __first, _ForwardIterator __last)
    {
        _M_len = __last - __first;
        _M_allocate_buffer();
        for (int __i = 0; __i < _M_len; ++__i)
            _M_buffer[__i] = *__first;                  // uninitialized_fill_n of *first
    }
    ~_Temporary_buffer() { free(_M_buffer); }
};
template <class _RandomAccessIter, class _Tp, class _Distance, class _Compare>
void __fastcall __stable_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Distance*, _Compare __comp)
{
    _Temporary_buffer<_RandomAccessIter, _Tp> buf(__first, __last);
    if (buf.begin() == 0)
        __inplace_stable_sort(__first, __last, __comp);
    else
        __stable_sort_adaptive(__first, __last, buf.begin(), _Distance(buf.size()), __comp);
}
}   // namespace std

typedef std::less<CSortElement> CLess;

// Test helpers: elements tagged with their original index so stability is checkable.
static CEngineSubPrimitiveBase* Tag(int i) { return (CEngineSubPrimitiveBase*)(size_t)(i + 1); }
static int TagOf(const CSortElement& e) { return (int)(size_t)e.SubPrimitive - 1; }
static unsigned long g_seed = 12345;
static unsigned long NextRand() { g_seed = g_seed * 1103515245UL + 12345UL; return (g_seed >> 8) & 0xFFFF; }
static void Fill(CSortElement* a, int n, unsigned long modulo)
{
    for (int i = 0; i < n; ++i) a[i] = CSortElement(NextRand() % modulo, Tag(i));
}
static bool IsSorted(const CSortElement* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[i].SortValue < a[i - 1].SortValue) return false;
    return true;
}
static bool IsStableSorted(const CSortElement* a, int n)
{
    for (int i = 1; i < n; ++i) {
        if (a[i].SortValue < a[i - 1].SortValue) return false;
        if (a[i].SortValue == a[i - 1].SortValue && TagOf(a[i]) < TagOf(a[i - 1])) return false;
    }
    return true;
}
static bool SameMultiset(const CSortElement* a, const CSortElement* b, int n)
{
    static int seen[4096];
    for (int i = 0; i < n; ++i) seen[i] = 0;
    for (int i = 0; i < n; ++i) {
        int t = TagOf(a[i]);
        if (t < 0 || t >= n || seen[t]) return false;
        seen[t] = 1;
        if (a[i].SortValue != b[t].SortValue) return false;
    }
    return true;
}

// Behaviour test for the STLport __merge_sort_loop<CSortElement*, less<CSortElement> > @ 0x00B858D0.
int main()
{
    static CSortElement a[101], r[101], o[101];
    for (int step = 1; step <= 16; step *= 2) {
        Fill(a, 101, 20);
        for (int i = 0; i < 101; ++i) o[i] = a[i];
        for (int s = 0; s < 101; s += step) {
            int e = s + step < 101 ? s + step : 101;
            std::__insertion_sort(a + s, a + e, CLess());
        }
        std::__merge_sort_loop(a, a + 101, r, step, CLess());
        for (int s = 0; s < 101; s += 2 * step) {
            int e = s + 2 * step < 101 ? s + 2 * step : 101;
            if (!IsStableSorted(r + s, e - s)) { std::printf("00b858d0_TEST FAIL\n"); return 1; }
        }
        if (!SameMultiset(r, o, 101)) { std::printf("00b858d0_TEST FAIL\n"); return 1; }
    }
    std::printf("00b858d0_TEST PASS\n");
    return 0;
}