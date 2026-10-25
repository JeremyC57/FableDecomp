// Behaviour test for 0x004D1BC0: STLport __partial_sort over a subtitled message's animation tags.
// Standalone: a copy of the model plus test versions of the out-of-line callees; checks the algorithm's
// post-conditions (order, same elements with their payloads, no leaked or double-freed strings).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// Test stand-in for CCharString: an owned C string (retail shares a CBasicString<char> block). The compare keeps
// retail's inline CCharString::operator< null rules and the CBasicString<char>::operator< 0x00429950 loop.
static int gLiveStrings = 0;
class CCharString {
public:
    char* PStringData;
    CCharString(const char* s) : PStringData(Dup(s)) { ++gLiveStrings; }
    CCharString(const CCharString& other) : PStringData(Dup(other.PStringData)) { ++gLiveStrings; }
    ~CCharString() { std::free(PStringData); --gLiveStrings; }
    CCharString& operator=(const CCharString& other)
    {
        if (this != &other) {
            char* p = Dup(other.PStringData);
            std::free(PStringData);
            PStringData = p;
        }
        return *this;
    }
    bool operator<(const CCharString& other) const
    {
        if (other.PStringData == PStringData)
            return false;
        if (PStringData == 0)
            return true;
        if (other.PStringData == 0)
            return false;
        const char* b = other.PStringData;
        const char* a = PStringData;
        for (;;) {
            if (*a == 0 && *b == 0)
                return false;
            if (*a < *b)
                return true;
            if (*a > *b)
                return false;
            ++a;
            ++b;
        }
    }
    static char* Dup(const char* s)
    {
        if (s == 0)
            return 0;
        unsigned int n = (unsigned int)std::strlen(s) + 1;
        char* p = (char*)std::malloc(n);
        std::memcpy(p, s, n);
        return p;
    }
};
static CCharString MkString(char prefix, int v)        // v < 0 -> the null string
{
    if (v < 0)
        return CCharString(0);
    char buf[16];
    if (prefix)
        std::sprintf(buf, "%c%d", prefix, v);
    else
        std::sprintf(buf, "%04d", v);
    return CCharString(buf);
}
static int StringValue(const CCharString& s, char prefix)
{
    if (s.PStringData == 0)
        return -1;
    if (prefix)
        return s.PStringData[0] == prefix ? std::atoi(s.PStringData + 1) : -1000;
    return std::atoi(s.PStringData);
}
static int LiveObjects() { return gLiveStrings; }

namespace _STL {
template <class _T1, class _T2> struct pair {          // STLport pair: user constructors, implicit operator=
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess { // empty functor, by value
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<float, CCharString> TElem;
typedef CKeyPairCompareLess<float, CCharString> TLess;
#define LOWEST_KEY 0
static TElem Mk(int key, int tag) { return TElem((float)key * 0.5f, MkString('t', tag)); }
static int KeyOf(const TElem& e) { return (int)(e.first * 2.0f + 0.25f); }
static int TagOf(const TElem& e) { return StringValue(e.second, 't'); }
static bool PayloadOk(const TElem& e) { return e.second.PStringData != 0; }

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

static bool TestFunction()                              // __partial_sort: the m smallest, sorted, in front
{
    bool ok = true;
    for (unsigned int seed = 1; seed <= 6 && ok; ++seed) {
        const int n = 30;
        const int m = 3 + (int)seed;
        int keys[n];
        Keys(keys, n, seed, 18, LOWEST_KEY);
        TElem* a = NewArr(keys, n);
        _STL::__partial_sort(a, a + m, a + n, (TElem*)0, TLess());
        ok = IsPerm(a, n, keys) && IsSorted(a, m);
        for (int i = m; ok && i < n; ++i)
            if (TLess()(a[i], a[m - 1]))
                ok = false;
        FreeArr(a, n);
    }
    return ok;
}

int main()
{
    int live = LiveObjects();
    bool ok = TestFunction();
    if (!ok || LiveObjects() != live) {
        std::printf("%s_TEST FAIL\n", "004d1bc0");
        return 1;
    }
    std::printf("%s_TEST PASS\n", "004d1bc0");
    return 0;
}