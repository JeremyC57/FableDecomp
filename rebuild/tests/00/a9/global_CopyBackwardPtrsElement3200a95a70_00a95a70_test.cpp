// Behaviour test for 0x00A95A70 CopyBackwardPtrs_Element32_00a95a70: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SElement32 {
    unsigned long d[8];
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

inline SElement32* __copy_backward(SElement32* first, SElement32* last, SElement32* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

SElement32* __fastcall CopyBackwardPtrs_Element32_00a95a70(SElement32* first, SElement32* last, SElement32* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}


typedef SElement32 T;
#define KEY(E) (E).d[0]
#define INIT(E) (E).d[7] = 5
#define CALL(f, l, r) CopyBackwardPtrs_Element32_00a95a70(f, l, r, ft)

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
    CHECK(a[3].d[7] == 5);
    if (g_fail) { std::printf("00a95a70_TEST FAIL\n"); return 1; }
    std::printf("00a95a70_TEST PASS\n");
    return 0;
}