// Behaviour test for 0x00769B90 Copy_StringStringVector_00769b90: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SStringStringVector {                 // 0x14 bytes
    CCharString First;                       // +0x00
    CCharString Second;                      // +0x04
    float       X;                           // +0x08
    float       Y;                           // +0x0C
    float       Z;                           // +0x10
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SStringStringVector* __fastcall Copy_StringStringVector_00769b90(SStringStringVector* first, SStringStringVector* last, SStringStringVector* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

static int g_strAssign = 0;
const CCharString& CCharString::operator=(const CCharString& other) { m_pData = other.m_pData; ++g_strAssign; return *this; }

typedef SStringStringVector T;
#define KEY(E) (E).X
#define INIT(E) (E).First.m_pData = &(E); (E).Second.m_pData = 0; (E).Z = 2.0f
#define CALL(f, l, r) Copy_StringStringVector_00769b90(f, l, r, tag, 0)

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
    CHECK(g_strAssign == 6 && b[1].First.m_pData == &a[1] && b[1].Z == 2.0f);
    if (g_fail) { std::printf("00769b90_TEST FAIL\n"); return 1; }
    std::printf("00769b90_TEST PASS\n");
    return 0;
}