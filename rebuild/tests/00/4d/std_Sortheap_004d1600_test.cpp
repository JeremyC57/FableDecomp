#include <cstdio>
// Behaviour test for 0x004D1600: STLport sort_heap over pair<float,CCharString>; checks the string payload follows its key and no string leaks.
#include <cstring>
#include <cstdlib>
static int g_live = 0;
struct CCharString {
    char* p;
    CCharString(const char* s) { p = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(p, s); ++g_live; }
    CCharString(const CCharString& o) { p = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(p, o.p); ++g_live; }
    ~CCharString() { std::free(p); --g_live; }
    CCharString& operator=(const CCharString& o)
    {
        if (this != &o) { char* n = (char*)std::malloc(std::strlen(o.p) + 1); std::strcpy(n, o.p); std::free(p); p = n; }
        return *this;
    }
};
struct Elem {
    float first; CCharString second;
    Elem(float f, const char* s) : first(f), second(s) { }
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
    {
        Elem v[6] = { Elem(3.0f, "three"), Elem(1.0f, "one"), Elem(5.0f, "five"), Elem(-1.0f, "minus"), Elem(2.0f, "two"), Elem(4.0f, "four") };
        int live0 = g_live;
        make_heap_by_push(v, 6, CKeyPairCompareLess());
        sort_heap(v, v + 6, CKeyPairCompareLess());
        if (g_live != live0) { std::printf("004d1600_TEST FAIL\n"); return 1; }
        const char* want[6] = { "minus", "one", "two", "three", "four", "five" };
        for (int i = 0; i < 6; ++i)
            if (std::strcmp(v[i].second.p, want[i]) != 0 || (i && !(v[i - 1].first < v[i].first))) { std::printf("004d1600_TEST FAIL\n"); return 1; }
    }
    if (g_live != 0) { std::printf("004d1600_TEST FAIL\n"); return 1; }
    std::printf("004d1600_TEST PASS\n");
    return 0;
}