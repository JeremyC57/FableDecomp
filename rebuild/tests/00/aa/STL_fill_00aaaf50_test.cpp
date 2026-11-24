// Behaviour test for 0x00AAAF50: std::fill<CPlane*> (NF382 name fix, was the fill<C3DClothQuad*> instance)
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

class C3DVector { public: float X, Y, Z; };
class CPlane { public: C3DVector Normal; float Distance; };
// the row: std::fill<CPlane*, CPlane>
static void __fastcall FillPlanes(CPlane* first, CPlane* last, const CPlane& val)
{
    for (; first != last; ++first)
        *first = val;
}
int main()
{
    Check(sizeof(CPlane) == (unsigned int)16);
    CPlane a[5];
    memset(a, 0, sizeof(a));
    CPlane v = { { 1.0f, 2.0f, 3.0f }, 8.0f };
    FillPlanes(a, a + 4, v);
    for (int k = 0; k < 4; ++k)
        Check(a[k].Normal.X == 1.0f && a[k].Normal.Z == 3.0f && a[k].Distance == 8.0f);
    Check(a[4].Normal.X == 0.0f && a[4].Distance == 0.0f);

    if (g_fail) {
        std::printf("00aaaf50_TEST FAIL\n");
        return 1;
    }
    std::printf("00aaaf50_TEST PASS\n");
    return 0;
}