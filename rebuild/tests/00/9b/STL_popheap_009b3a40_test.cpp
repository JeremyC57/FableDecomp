// Behaviour test for 0x009B3A40 _STL::__pop_heap<CWideString*, CWideString, less<CWideString>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("009b3a40_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CWideString {                                     // test model: a short owned text
public:
    char Text[16];
    CWideString() { Text[0] = 0; }
    CWideString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CWideString(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CWideString() { ++g_dtors; }
    CWideString& operator=(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return std::strcmp(a.Text, b.Text) < 0; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void push_heap_model(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp)
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
void adjust_heap_model(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
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
    push_heap_model(__first, __holeIndex, __topIndex, __val, __comp);
}
// the function under test: STLport __pop_heap
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void pop_heap_under_test(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    adjust_heap_model(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _Tp, class _Compare>
void push_heap_all(_Tp* __first, _Tp* __last, _Compare __comp)
{
    push_heap_model(__first, int((__last - __first) - 1), int(0), _Tp(*(__last - 1)), __comp);
}
template <class _Tp, class _Compare>
void pop_heap_all(_Tp* __first, _Tp* __last, _Compare __comp)
{
    pop_heap_under_test(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}

int main()
{
    int i;
    CWideString v[7] = { CWideString("m"), CWideString("c"), CWideString("x"), CWideString("a"), CWideString("q"), CWideString("c"), CWideString("z") };
    for (i = 1; i <= 7; ++i) push_heap_all(v, v + i, less<CWideString>());
    for (i = 7; i > 1; --i) pop_heap_all(v, v + i, less<CWideString>());
    for (i = 0; i + 1 < 7; ++i) CHECK(!less<CWideString>()(v[i + 1], v[i]));
    CHECK(v[0].Is("a") && v[1].Is("c") && v[2].Is("c") && v[3].Is("m") && v[4].Is("q") && v[5].Is("x") && v[6].Is("z"));
    std::printf("009b3a40_TEST PASS\n");
    return 0;
}