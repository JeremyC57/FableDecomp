// Behaviour test for 0x009929C0 CopyBackward_PairLongRecord20_009929c0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SRecord20 {                           // 0x20 bytes
    unsigned char _body[0x20];
    SRecord20& operator=(const SRecord20& other);         // 0x00992630
};

struct SPairLongRecord20 {                   // 0x24 bytes
    long      first;                         // +0x00
    SRecord20 second;                        // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SPairLongRecord20* __fastcall CopyBackward_PairLongRecord20_009929c0(SPairLongRecord20* first, SPairLongRecord20* last, SPairLongRecord20* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_recAssign = 0;
SRecord20& SRecord20::operator=(const SRecord20& other) { memcpy(_body, other._body, sizeof(_body)); ++g_recAssign; return *this; }

typedef SPairLongRecord20 T;
#define KEY(E) (E).first
#define INIT(E) (E).second._body[0] = 8
#define CALL(f, l, r) CopyBackward_PairLongRecord20_009929c0(f, l, r, tag, 0)

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
    CHECK(g_recAssign == 3 && a[3].second._body[0] == 8);
    if (g_fail) { std::printf("009929c0_TEST FAIL\n"); return 1; }
    std::printf("009929c0_TEST PASS\n");
    return 0;
}