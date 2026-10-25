// Behaviour test for 0x0081BE30 std::__linear_insert over 0x28-byte records keyed by a signed long: a value below the
// first key shifts [first, last) up one slot (copy_backward) and goes first; otherwise the unguarded insert walks it
// down from last. Working stand-ins for copy_backward and __unguarded_linear_insert.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace std {

struct SRecordBody24 { long Words[9]; };
struct SKeyedRecord28 {
    long          Key;
    SRecordBody24 Body;
    SKeyedRecord28() { }                               // test-only: lets the test build an array
    SKeyedRecord28(const SKeyedRecord28& r) : Key(r.Key), Body(r.Body) { }
};
struct CKeyCompareLess {
    bool operator()(const SKeyedRecord28& a, const SKeyedRecord28& b) const { return a.Key < b.Key; }
};

static int g_backward = 0, g_unguarded = 0;
SKeyedRecord28* __fastcall copy_backward(SKeyedRecord28* first, SKeyedRecord28* last, SKeyedRecord28* result)
{
    ++g_backward;
    while (last != first) *--result = *--last;
    return result;
}
void __fastcall __unguarded_linear_insert(SKeyedRecord28* last, SKeyedRecord28 val, CKeyCompareLess comp)
{
    ++g_unguarded;
    SKeyedRecord28* next = last - 1;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

void __fastcall __linear_insert(SKeyedRecord28* first, SKeyedRecord28* last, SKeyedRecord28 val, CKeyCompareLess comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

}

int main()
{
    using namespace std;
    SKeyedRecord28 a[5];
    long keys[5] = { -10, 20, 30, -50, 25 };
    for (int i = 0; i < 5; ++i) { a[i].Key = keys[i]; a[i].Body.Words[8] = keys[i] * 3; }
    CKeyCompareLess comp;
    __linear_insert(a, a + 3, a[3], comp);             // -50 < -10 (signed): shift and put first
    __linear_insert(a, a + 4, a[4], comp);             // 25: unguarded insert
    long want[5] = { -50, -10, 20, 25, 30 };
    int ok = g_backward == 1 && g_unguarded == 1;
    for (int i = 0; i < 5; ++i) ok = ok && a[i].Key == want[i] && a[i].Body.Words[8] == want[i] * 3;
    if (!ok) { std::printf("0081be30_TEST FAIL\n"); return 1; }
    std::printf("0081be30_TEST PASS\n");
    return 0;
}