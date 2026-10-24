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

struct C3DTriangle2 { unsigned short VertexIndex[3]; };
// the row: _STL::uninitialized_fill_n<C3DTriangle2*, unsigned int, C3DTriangle2> -- placement copies, returns the end
static C3DTriangle2* __fastcall UninitializedFillN(C3DTriangle2* first, unsigned int n, const C3DTriangle2& x)
{
    C3DTriangle2* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) C3DTriangle2(x);
    return cur;
}
int main()
{
    C3DTriangle2 buf[8];
    memset(buf, 0, sizeof(buf));
    C3DTriangle2 v = { { 9, 8, 7 } };
    C3DTriangle2* end = UninitializedFillN(buf + 2, 5, v);
    Check(end == buf + 7);
    for (int k = 2; k < 7; ++k)
        Check(buf[k].VertexIndex[0] == 9 && buf[k].VertexIndex[1] == 8 && buf[k].VertexIndex[2] == 7);
    Check(buf[1].VertexIndex[0] == 0 && buf[7].VertexIndex[0] == 0);
    Check(UninitializedFillN(buf, 0, v) == buf && buf[0].VertexIndex[0] == 0);

    if (g_fail) {
        std::printf("00a95970_TEST FAIL\n");
        return 1;
    }
    std::printf("00a95970_TEST PASS\n");
    return 0;
}