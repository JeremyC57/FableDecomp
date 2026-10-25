// Behaviour test for 0x00758E00 std::__linear_insert over 0x2C-byte records keyed by a signed long: a value below the
// first key shifts [first, last) up one slot (copy_backward) and goes first; otherwise the unguarded insert walks it
// down from last. Working stand-ins for copy_backward and __unguarded_linear_insert.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace std {

struct SRecordBody28 { long Words[10]; };
struct SKeyedRecord2C {
    long          Key;
    SRecordBody28 Body;
    SKeyedRecord2C() { }                               // test-only: lets the test build an array
    SKeyedRecord2C(const SKeyedRecord2C& r) : Key(r.Key), Body(r.Body) { }
};
struct CKeyCompareLess {
    bool operator()(const SKeyedRecord2C& a, const SKeyedRecord2C& b) const { return a.Key < b.Key; }
};

static int g_backward = 0, g_unguarded = 0;
SKeyedRecord2C* __fastcall copy_backward(SKeyedRecord2C* first, SKeyedRecord2C* last, SKeyedRecord2C* result)
{
    ++g_backward;
    while (last != first) *--result = *--last;
    return result;
}
void __fastcall __unguarded_linear_insert(SKeyedRecord2C* last, SKeyedRecord2C val, CKeyCompareLess comp)
{
    ++g_unguarded;
    SKeyedRecord2C* next = last - 1;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}

void __fastcall __linear_insert(SKeyedRecord2C* first, SKeyedRecord2C* last, SKeyedRecord2C val, CKeyCompareLess comp)
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
    SKeyedRecord2C a[5];
    long keys[5] = { -10, 20, 30, -50, 25 };
    for (int i = 0; i < 5; ++i) { a[i].Key = keys[i]; a[i].Body.Words[9] = keys[i] * 3; }
    CKeyCompareLess comp;
    __linear_insert(a, a + 3, a[3], comp);             // -50 < -10 (signed): shift and put first
    __linear_insert(a, a + 4, a[4], comp);             // 25: unguarded insert
    long want[5] = { -50, -10, 20, 25, 30 };
    int ok = g_backward == 1 && g_unguarded == 1;
    for (int i = 0; i < 5; ++i) ok = ok && a[i].Key == want[i] && a[i].Body.Words[9] == want[i] * 3;
    if (!ok) { std::printf("00758e00_TEST FAIL\n"); return 1; }
    std::printf("00758e00_TEST PASS\n");
    return 0;
}