// Behaviour test for 0x008893D0 CopyBackward_Record30_008893d0: a copy of the source model with working stand-ins for the out-of-line callees.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct SVector {                             // 12-byte STL vector (first, last, end of storage)
    void* m_pFirst;                          // +0x00
    void* m_pLast;                           // +0x04
    void* m_pEnd;                            // +0x08
    SVector& operator=(const SVector& other);              // 0x00888EE0
};

struct SRecord30 {                           // 0x30 bytes
    long    Value0;                          // +0x00
    bool    Flag0;                           // +0x04
    SVector Items;                           // +0x08
    long    Value1;                          // +0x14
    long    Value2;                          // +0x18
    bool    Flag1;                           // +0x1C
    double  Time;                            // +0x20
    long    Value3;                          // +0x28
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SRecord30* __fastcall CopyBackward_Record30_008893d0(SRecord30* first, SRecord30* last, SRecord30* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

static int g_vecAssign = 0;
SVector& SVector::operator=(const SVector& other) { m_pFirst = other.m_pFirst; m_pLast = other.m_pLast; m_pEnd = other.m_pEnd; ++g_vecAssign; return *this; }

typedef SRecord30 T;
#define KEY(E) (E).Value0
#define INIT(E) (E).Items.m_pFirst = &(E); (E).Time = 2.5; (E).Flag1 = true
#define CALL(f, l, r) CopyBackward_Record30_008893d0(f, l, r, tag, 0)

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
    CHECK(g_vecAssign == 3 && a[3].Items.m_pFirst == &a[2] && a[3].Time == 2.5);
    if (g_fail) { std::printf("008893d0_TEST FAIL\n"); return 1; }
    std::printf("008893d0_TEST PASS\n");
    return 0;
}