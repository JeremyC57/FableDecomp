// Behaviour test for 0x00A522B0 CopyBackward_VectorWord_00a522b0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct C3DVectorWord {                 // C3DAnimationSequenceData::C3DVectorWord, 6 bytes
    short X;                           // +0
    short Y;                           // +2
    short Z;                           // +4
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

C3DVectorWord* __fastcall CopyBackward_VectorWord_00a522b0(C3DVectorWord* first, C3DVectorWord* last, C3DVectorWord* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}


typedef C3DVectorWord T;
#define KEY(E) (E).X
#define INIT(E) (E).Y = 3; (E).Z = 4
#define CALL(f, l, r) CopyBackward_VectorWord_00a522b0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[5];
    for (i = 0; i < 5; ++i) { INIT(a[i]); KEY(a[i]) = i + 1; }
    T* r = CALL(a, a + 3, a + 4);
    CHECK(r == a + 1);
    CHECK(KEY(a[0]) == 1 && KEY(a[1]) == 1 && KEY(a[2]) == 2 && KEY(a[3]) == 3 && KEY(a[4]) == 5);
    r = CALL(a, a, a + 2);
    CHECK(r == a + 2);
    CHECK(a[3].Y == 3 && a[3].Z == 4);
    if (g_fail) { std::printf("00a522b0_TEST FAIL\n"); return 1; }
    std::printf("00a522b0_TEST PASS\n");
    return 0;
}