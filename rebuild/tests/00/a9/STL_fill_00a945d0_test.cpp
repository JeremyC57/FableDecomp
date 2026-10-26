#include <cstdio>
#include <new>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
typedef unsigned long long u64;
#else
typedef unsigned __int64 u64;
#endif

static int g_fail = 0;
static void Check(bool ok) { if (!ok) g_fail = 1; }

class C3DTriangle2 { public: unsigned short VertexIndex[3]; };
// the row: std::fill<C3DTriangle2*, C3DTriangle2>
static void __fastcall FillTriangles(C3DTriangle2* first, C3DTriangle2* last, const C3DTriangle2& val)
{
    for (; first != last; ++first)
        *first = val;
}
int main()
{
    Check(sizeof(C3DTriangle2) == (unsigned int)6);
    C3DTriangle2 a[6];
    memset(a, 0xEE, sizeof(a));
    C3DTriangle2 v = { { 1, 2, 3 } };
    FillTriangles(a + 1, a + 5, v);
    for (int k = 1; k < 5; ++k)
        Check(a[k].VertexIndex[0] == 1 && a[k].VertexIndex[1] == 2 && a[k].VertexIndex[2] == 3);
    Check(a[0].VertexIndex[0] == 0xEEEE && a[5].VertexIndex[2] == 0xEEEE);
    FillTriangles(a, a, v);
    Check(a[0].VertexIndex[0] == 0xEEEE);

    if (g_fail) {
        std::printf("00a945d0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a945d0_TEST PASS\n");
    return 0;
}