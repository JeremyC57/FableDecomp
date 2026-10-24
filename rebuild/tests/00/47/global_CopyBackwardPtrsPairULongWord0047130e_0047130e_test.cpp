// Behaviour test for 0x0047130E CopyBackwardPtrs_PairULongWord_0047130e: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)

struct SPairULongWord {
    unsigned long first;               // +0x00 sort key
    unsigned long second;              // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

__forceinline SPairULongWord* __copy_backward(SPairULongWord* first, SPairULongWord* last, SPairULongWord* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

SPairULongWord* __fastcall CopyBackwardPtrs_PairULongWord_0047130e(SPairULongWord* first, SPairULongWord* last, SPairULongWord* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}


typedef SPairULongWord T;
#define KEY(E) (E).first
#define INIT(E) (E).second = 0
#define CALL(f, l, r) CopyBackwardPtrs_PairULongWord_0047130e(f, l, r, ft)

int main()
{
    int i;
    __false_type ft;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    a[0].second = 77; r = CALL(a, a + 1, a + 2); CHECK(r == a + 1 && a[1].second == 77);
    if (g_fail) { std::printf("0047130e_TEST FAIL\n"); return 1; }
    std::printf("0047130e_TEST PASS\n");
    return 0;
}