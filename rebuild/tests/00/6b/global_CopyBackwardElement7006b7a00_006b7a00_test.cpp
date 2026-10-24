// Behaviour test for 0x006B7A00 CopyBackward_Element7_006b7a00: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SElement7 {                     // 7-byte record (stride 7, no alignment padding)
    unsigned char _body[7];
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SElement7* __fastcall CopyBackward_Element7_006b7a00(SElement7* first, SElement7* last, SElement7* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}


typedef SElement7 T;
#define KEY(E) (E)._body[0]
#define INIT(E) memset((E)._body, 9, 7)
#define CALL(f, l, r) CopyBackward_Element7_006b7a00(f, l, r, tag, 0)

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
    CHECK(a[3]._body[6] == 9);
    if (g_fail) { std::printf("006b7a00_TEST FAIL\n"); return 1; }
    std::printf("006b7a00_TEST PASS\n");
    return 0;
}