// Behaviour test for 0x009B1C30 __push_heap over CWideString: a value pushed at the hole climbs past smaller parents (max-heap under less) and stops at topIndex.
#include <cstdio>
#include <cwchar>
static int g_live = 0;
class CWideString {                                 // value model: owns a copy of a short wide string
public:
    wchar_t Text[16];
    CWideString(const wchar_t* s) { ++g_live; std::wcscpy(Text, s); }
    CWideString(const CWideString& o) { ++g_live; std::wcscpy(Text, o.Text); }
    ~CWideString() { --g_live; }
    const CWideString& operator=(const CWideString& o) { std::wcscpy(Text, o.Text); return *this; }
    bool operator<(const CWideString& o) const { return std::wcscmp(Text, o.Text) < 0; }
};
template <class _Tp> struct less {
    bool operator()(const _Tp& x, const _Tp& y) const { return x < y; }
};
template <class _RandomAccessIter, class _Tp, class _Compare>
void __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last; --__next;
    while (__comp(__val, *__next)) { *__last = *__next; __last = __next; --__next; }
    *__last = __val;
}
template <class _RI, class _Distance, class _Tp, class _Compare>
void __push_heap(_RI __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __val;
}
template <class _RI, class _Distance, class _Tp, class _Compare>
void __adjust_heap(_RI __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1)))) __secondChild--;
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
template <class _RI, class _Compare, class _Tp, class _Distance>
void __make_heap(_RI __first, _RI __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = (_Distance)(__last - __first);
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template <class _RI, class _Tp, class _Compare>
_RI __unguarded_partition(_RI __first, _RI __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot)) ++__first;
        --__last;
        while (__comp(__pivot, *__last)) --__last;
        if (!(__first < __last)) return __first;
        _Tp __tmp = *__first; *__first = *__last; *__last = __tmp;
        ++__first;
    }
}
template <class _RI, class _Tp, class _Compare>
void __linear_insert(_RI __first, _RI __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        for (int __n = (int)(__last - __first); __n > 0; --__n) { *__last = *(__last - 1); --__last; }
        *__first = __val;
    }
    else __unguarded_linear_insert(__last, __val, __comp);
}
template <class _RI, class _Compare>
void __insertion_sort(_RI __first, _RI __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RI __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, CWideString(*__i), __comp);
}
static const wchar_t* W[8] = { L"delta", L"alpha", L"hotel", L"charlie", L"bravo", L"golf", L"echo", L"foxtrot" };
static bool sorted(CWideString* a, int n) { for (int i = 1; i < n; ++i) if (a[i] < a[i - 1]) return false; return true; }
static bool isheap(CWideString* a, int n) { for (int i = 1; i < n; ++i) if (a[(i - 1) / 2] < a[i]) return false; return true; }
#define FAIL(a) { std::printf(a "_TEST FAIL\n"); return 1; }
int main()
{
    {
        CWideString h[8] = { CWideString(L"golf"), CWideString(L"echo"), CWideString(L"foxtrot"), CWideString(L"alpha"),
                             CWideString(L"bravo"), CWideString(L"charlie"), CWideString(L"delta"), CWideString(L"zulu") };
        if (!isheap(h, 7)) FAIL("009b1c30")
        __push_heap(h, 7, 0, CWideString(h[7]), less<CWideString>());
        if (!isheap(h, 8) || std::wcscmp(h[0].Text, L"zulu") != 0) FAIL("009b1c30")
        // topIndex bounds the climb: hole 4 with top 1 stops at index 1
        CWideString k[5] = { CWideString(L"b"), CWideString(L"c"), CWideString(L"d"), CWideString(L"e"), CWideString(L"z") };
        __push_heap(k, 4, 1, CWideString(L"z"), less<CWideString>());
        if (std::wcscmp(k[1].Text, L"z") != 0 || std::wcscmp(k[4].Text, L"c") != 0 || std::wcscmp(k[0].Text, L"b") != 0) FAIL("009b1c30")
    }
    if (g_live != 0) FAIL("009b1c30")
    std::printf("009b1c30_TEST PASS\n");
    return 0;
}