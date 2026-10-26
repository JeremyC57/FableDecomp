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

class C3DClothQuad { public: unsigned short VertexIndex[4]; unsigned short TexCoordIndex[4]; };
// the row: std::fill<C3DClothQuad*, C3DClothQuad>
static void __fastcall FillQuads(C3DClothQuad* first, C3DClothQuad* last, const C3DClothQuad& val)
{
    for (; first != last; ++first)
        *first = val;
}
int main()
{
    Check(sizeof(C3DClothQuad) == (unsigned int)16);
    C3DClothQuad a[5];
    memset(a, 0, sizeof(a));
    C3DClothQuad v = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 } };
    FillQuads(a, a + 4, v);
    for (int k = 0; k < 4; ++k)
        Check(a[k].VertexIndex[0] == 1 && a[k].VertexIndex[3] == 4 && a[k].TexCoordIndex[0] == 5 && a[k].TexCoordIndex[3] == 8);
    Check(a[4].VertexIndex[0] == 0 && a[4].TexCoordIndex[3] == 0);

    if (g_fail) {
        std::printf("00aaaf50_TEST FAIL\n");
        return 1;
    }
    std::printf("00aaaf50_TEST PASS\n");
    return 0;
}