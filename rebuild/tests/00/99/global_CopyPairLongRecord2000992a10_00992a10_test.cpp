// Behaviour test for 0x00992A10 Copy_PairLongRecord20_00992a10: a copy of the source model with working stand-ins for the out-of-line callees.
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

SPairLongRecord20* __fastcall Copy_PairLongRecord20_00992a10(SPairLongRecord20* first, SPairLongRecord20* last, SPairLongRecord20* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static int g_recAssign = 0;
SRecord20& SRecord20::operator=(const SRecord20& other) { memcpy(_body, other._body, sizeof(_body)); ++g_recAssign; return *this; }

typedef SPairLongRecord20 T;
#define KEY(E) (E).first
#define INIT(E) (E).second._body[0] = 8
#define CALL(f, l, r) Copy_PairLongRecord20_00992a10(f, l, r, tag, 0)

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
    CHECK(g_recAssign == 3 && b[2].second._body[0] == 8);
    if (g_fail) { std::printf("00992a10_TEST FAIL\n"); return 1; }
    std::printf("00992a10_TEST PASS\n");
    return 0;
}