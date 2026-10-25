// Behaviour test for 0x00A52080: std::fill<C3DTriangle2*> (assign one triangle across a range)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];
};
static C3DTriangle2 Tri(unsigned short a, unsigned short b, unsigned short c)
{
    C3DTriangle2 t;
    t.VertexIndex[0] = a; t.VertexIndex[1] = b; t.VertexIndex[2] = c;
    return t;
}
static bool IsTri(const C3DTriangle2& t, unsigned short a, unsigned short b, unsigned short c)
{
    return t.VertexIndex[0] == a && t.VertexIndex[1] == b && t.VertexIndex[2] == c;
}

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}

int main()
{
    C3DTriangle2 a[5];
    for (int i = 0; i < 5; ++i)
        a[i] = Tri(0, 0, 0);
    C3DTriangle2 v = Tri(7, 8, 65535);
    std::fill(a + 1, a + 4, v);
    Check(IsTri(a[0], 0, 0, 0) && IsTri(a[4], 0, 0, 0));
    for (int j = 1; j < 4; ++j)
        Check(IsTri(a[j], 7, 8, 65535));
    std::fill(a, a, Tri(1, 1, 1));
    Check(IsTri(a[0], 0, 0, 0));
    if (g_fail) {
        printf("00a52080_TEST FAIL\n");
        return 1;
    }
    printf("00a52080_TEST PASS\n");
    return 0;
}