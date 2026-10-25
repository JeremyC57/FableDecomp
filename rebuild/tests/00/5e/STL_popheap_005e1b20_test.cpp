// Behaviour test for 0x005E1B20 std::__pop_heap<CCharString*, CCharString, less<CCharString>>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("005e1b20_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CCharString {                                     // test model: a short owned text
public:
    char Text[16];
    CCharString() { Text[0] = 0; }
    CCharString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CCharString(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CCharString() { ++g_dtors; }
    CCharString& operator=(const CCharString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
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
    CCharString v[5] = { CCharString("pear"), CCharString("apple"), CCharString("fig"), CCharString("plum"), CCharString("date") };
    for (i = 1; i <= 5; ++i) push_heap_all(v, v + i, less<CCharString>());
    for (i = 5; i > 1; --i) pop_heap_all(v, v + i, less<CCharString>());
    for (i = 0; i + 1 < 5; ++i) CHECK(!less<CCharString>()(v[i + 1], v[i]));
    CHECK(v[0].Is("apple") && v[1].Is("date") && v[2].Is("fig") && v[3].Is("pear") && v[4].Is("plum"));
    std::printf("005e1b20_TEST PASS\n");
    return 0;
}