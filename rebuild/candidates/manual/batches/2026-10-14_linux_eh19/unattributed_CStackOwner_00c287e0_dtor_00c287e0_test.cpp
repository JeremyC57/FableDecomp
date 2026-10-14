#include <cstdio>
#include <new>
static int g_c = 0, g_m = 0, g_f = 0; struct Li { virtual void v0() {} ~Li() {} }; struct Vc { void* b; void* e; void* c; ~Vc() { if (b) { ++g_f; } b = 0; e = 0; c = 0; } }; struct Mb { unsigned char b[4]; ~Mb() { ++g_m; } };
struct X { virtual void v0() {} unsigned char p[0x30]; Li li; unsigned char q[0x408]; Vc v; long m44c; Mb m; void Cl() { ++g_c; } ~X() { Cl(); } };
static bool Check() { X* x = new X; int a; x->v.b = &a; x->v.e = &a; x->v.c = &a; delete x; return g_c == 1 && g_m == 1 && g_f == 1; }

int main()
{
    if (!Check()) { std::printf("00c287e0_TEST FAIL\n"); return 1; }
    std::printf("00c287e0_TEST PASS\n");
    return 0;
}
