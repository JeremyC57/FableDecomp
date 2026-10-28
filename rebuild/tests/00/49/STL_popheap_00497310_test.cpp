// Behaviour test for 0x00497310 std::__pop_heap<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00497310_TEST FAIL\n"); return 1; } } while (0)

static int g_copies = 0, g_assigns = 0, g_dtors = 0;
class CWideString {                                     // test model: a short owned text
public:
    char Text[16];
    CWideString() { Text[0] = 0; }
    CWideString(const char* s) { std::strncpy(Text, s, 15); Text[15] = 0; }
    CWideString(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_copies; }
    ~CWideString() { ++g_dtors; }
    const CWideString& operator=(const CWideString& o) { std::memcpy(Text, o.Text, 16); ++g_assigns; return *this; }
    bool Is(const char* s) const { return std::strcmp(Text, s) == 0; }
};

namespace NGameComponentHelpers {
class CCaseInsensitiveStringCompare {
public:
    bool operator()(const CWideString& a, const CWideString& b) const
    {
        int i;
        for (i = 0; ; ++i) {
            char ca = a.Text[i], cb = b.Text[i];
            if (ca >= 'A' && ca <= 'Z') ca = (char)(ca - 'A' + 'a');
            if (cb >= 'A' && cb <= 'Z') cb = (char)(cb - 'A' + 'a');
            if (ca != cb) return ca < cb;
            if (ca == 0) return false;
        }
    }
};
}

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
    CWideString v[6] = { CWideString("delta"), CWideString("Alpha"), CWideString("echo"), CWideString("Charlie"), CWideString("bravo"), CWideString("Foxtrot") };
    for (i = 1; i <= 6; ++i) push_heap_all(v, v + i, NGameComponentHelpers::CCaseInsensitiveStringCompare());
    for (i = 6; i > 1; --i) pop_heap_all(v, v + i, NGameComponentHelpers::CCaseInsensitiveStringCompare());
    for (i = 0; i + 1 < 6; ++i) CHECK(!NGameComponentHelpers::CCaseInsensitiveStringCompare()(v[i + 1], v[i]));
    CHECK(v[0].Is("Alpha") && v[1].Is("bravo") && v[2].Is("Charlie") && v[3].Is("delta") && v[4].Is("echo") && v[5].Is("Foxtrot"));
    std::printf("00497310_TEST PASS\n");
    return 0;
}