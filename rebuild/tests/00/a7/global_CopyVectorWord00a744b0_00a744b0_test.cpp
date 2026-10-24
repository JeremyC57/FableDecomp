// Behaviour test for 0x00A744B0 Copy_VectorWord_00a744b0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct C3DVectorWord {                 // C3DAnimationSequenceData::C3DVectorWord, 6 bytes (quantised x/y/z)
    short X;                           // +0
    short Y;                           // +2
    short Z;                           // +4
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

C3DVectorWord* __fastcall Copy_VectorWord_00a744b0(C3DVectorWord* first, C3DVectorWord* last, C3DVectorWord* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}


typedef C3DVectorWord T;
#define KEY(E) (E).X
#define INIT(E) (E).Y = 3; (E).Z = 4
#define CALL(f, l, r) Copy_VectorWord_00a744b0(f, l, r, tag, 0)

int main()
{
    int i;
    random_access_iterator_tag tag;
    T a[4];
    T b[4];
    for (i = 0; i < 4; ++i) { INIT(a[i]); INIT(b[i]); KEY(a[i]) = i + 1; KEY(b[i]) = 0; }
    T* r = CALL(a, a + 3, b);
    CHECK(r == b + 3);
    CHECK(KEY(b[0]) == 1 && KEY(b[1]) == 2 && KEY(b[2]) == 3 && KEY(b[3]) == 0);
    r = CALL(a, a, b + 1);
    CHECK(r == b + 1);
    CHECK(b[2].Y == 3 && b[2].Z == 4);
    if (g_fail) { std::printf("00a744b0_TEST FAIL\n"); return 1; }
    std::printf("00a744b0_TEST PASS\n");
    return 0;
}