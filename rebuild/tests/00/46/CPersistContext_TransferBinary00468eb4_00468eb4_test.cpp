#include <cstdio>
static int g_w = 0, g_r = 0; struct O; struct I; struct Vl;
static void __fastcall W(O*, Vl*) { ++g_w; } static void __fastcall R(I*, Vl*) { ++g_r; }
struct C { unsigned char p0[0x18]; long m; unsigned char p1[8]; I* in; O* out; void CRC(const char*) {} void T(Vl* v) { CRC(""); switch (m) { case 2: R(in, v); break; default: W(out, v); break; } } };
int main()
{
    C c; c.m = 2; c.T(0); c.m = 1; c.T(0); if (g_w != 1 || g_r != 1) { std::printf("00468eb4_TEST FAIL\n"); return 1; }

    std::printf("00468eb4_TEST PASS\n");
    return 0;
}