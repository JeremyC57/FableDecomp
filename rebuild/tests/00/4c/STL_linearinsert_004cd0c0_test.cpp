// Behaviour test for 0x004CD0C0: STLport __linear_insert over CThing::Components entries.
// Standalone: a copy of the model plus test versions of the out-of-line callees; checks the algorithm's
// post-conditions (order, same elements with their payloads, no leaked or double-freed strings).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int LiveObjects() { return 0; }

namespace std {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // empty functor, by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

enum ETCInterfaceType { TC_NONE = 0, TC_TEST_MIN = -128, TC_TEST_MAX = 127 };   // test range widened for negative keys
class CTCBase;
static char gThings[256];                               // fake component addresses
typedef std::pair<ETCInterfaceType, CTCBase*> TElem;
typedef CKeyPairCompareLess<ETCInterfaceType, CTCBase*> TLess;
#define LOWEST_KEY (-10)
static TElem Mk(int key, int tag) { return TElem((ETCInterfaceType)key, (CTCBase*)(gThings + tag)); }
static int KeyOf(const TElem& e) { return (int)e.first; }
static int TagOf(const TElem& e) { return (int)((char*)e.second - gThings); }
static bool PayloadOk(const TElem&) { return true; }

// STLport 4.x sort / heap algorithms (the model's bodies and the test's copies of their out-of-line callees).
namespace std {
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

// Generic checks over the element traits above (Mk / KeyOf / TagOf / PayloadOk).
static TElem* NewArr(const int* keys, int n)
{
    TElem* p = (TElem*)std::malloc((n + 1) * sizeof(TElem));
    for (int i = 0; i < n; ++i)
        new (p + i) TElem(Mk(keys[i], i));
    return p;
}
static void FreeArr(TElem* p, int n)
{
    for (int i = 0; i < n; ++i)
        p[i].~TElem();
    std::free(p);
}
static bool IsPerm(const TElem* a, int n, const int* keys)
{
    static int seen[256];
    for (int i = 0; i < n; ++i)
        seen[i] = 0;
    for (int i = 0; i < n; ++i) {
        int t = TagOf(a[i]);
        if (t < 0 || t >= n || seen[t] || KeyOf(a[i]) != keys[t] || !PayloadOk(a[i]))
            return false;
        seen[t] = 1;
    }
    return true;
}
static bool IsSorted(const TElem* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (TLess()(a[i], a[i - 1]))
            return false;
    return true;
}
static bool IsStable(const TElem* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (KeyOf(a[i]) == KeyOf(a[i - 1]) && TagOf(a[i]) < TagOf(a[i - 1]))
            return false;
    return true;
}
static bool IsHeap(const TElem* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (TLess()(a[(i - 1) / 2], a[i]))
            return false;
    return true;
}
static void Keys(int* keys, int n, unsigned int seed, int range, int lowest)
{
    unsigned int x = seed;
    for (int i = 0; i < n; ++i) {
        x = x * 1103515245u + 12345u;
        keys[i] = lowest + (int)((x >> 16) % (unsigned int)range);
    }
}

static bool TestFunction()                              // __linear_insert: new minimum goes to the front, else unguarded
{
    static const int keys[6] = { 3, 5, 5, 9, 12, 1 };
    static const int want[6] = { 5, 0, 1, 2, 3, 4 };
    static const int keys2[6] = { 3, 5, 5, 9, 12, 5 };
    static const int want2[6] = { 0, 1, 2, 5, 3, 4 };
    bool ok = true;
    TElem* a = NewArr(keys, 6);
    {
        TElem val(a[5]);
        std::__linear_insert(a, a + 5, TElem(val), TLess());
    }
    ok = IsPerm(a, 6, keys) && IsSorted(a, 6);
    for (int i = 0; ok && i < 6; ++i)
        if (TagOf(a[i]) != want[i])
            ok = false;
    FreeArr(a, 6);
    TElem* b = NewArr(keys2, 6);
    {
        TElem val(b[5]);
        std::__linear_insert(b, b + 5, TElem(val), TLess());
    }
    ok = ok && IsPerm(b, 6, keys2) && IsSorted(b, 6) && IsStable(b, 6);
    for (int i = 0; ok && i < 6; ++i)
        if (TagOf(b[i]) != want2[i])
            ok = false;
    FreeArr(b, 6);
    return ok;
}

int main()
{
    int live = LiveObjects();
    bool ok = TestFunction();
    if (!ok || LiveObjects() != live) {
        std::printf("%s_TEST FAIL\n", "004cd0c0");
        return 1;
    }
    std::printf("%s_TEST PASS\n", "004cd0c0");
    return 0;
}