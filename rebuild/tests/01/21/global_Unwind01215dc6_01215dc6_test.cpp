#include <cstdio>
#include <new>
static int g_c = 0, g_f = 0, g_s = 0, g_m = 0; struct Fi { unsigned char b[0x1c]; ~Fi() { ++g_f; } }; struct Se { unsigned char b[4]; ~Se() { ++g_s; } }; struct Pa { Fi f; Se s; }; struct Mb { unsigned char b[0x14]; ~Mb() { ++g_m; } };
struct X { virtual void v0() {} char* b1; unsigned char p[0x14]; char* b2; unsigned char q[0x14]; Mb m; Pa pr; void Cl() { ++g_c; } ~X() { Cl(); delete b1; delete b2; } };
static bool Check() { X* x = new X; x->b1 = new char; x->b2 = new char; delete x; return g_c == 1 && g_f == 1 && g_s == 1 && g_m == 1; }

int main()
{
    if (!Check()) { std::printf("00c4b270_TEST FAIL\n"); return 1; }
    std::printf("01215dc6_TEST PASS\n");
    return 0;
}