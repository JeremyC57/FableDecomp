#include <cstdio>
#include <new>
static int g_base = 0, g_snd = 0, g_el = 0, g_reg = 0; static long g_last = 0; struct Bs { virtual void v0() {} unsigned char b[0xb0]; Bs() { ++g_base; } ~Bs() {} }; struct Sn { unsigned char b[0x28]; Sn() { ++g_snd; } ~Sn() {} }; struct El { unsigned char b[0x24]; El() { ++g_el; } }; struct Tb {}; struct Mt { void Register(Tb* t, long s, long a, long b, long c, long d) { ++g_reg; g_last = b; } }; static Mt g_ma, g_mb; static Tb g_ta, g_tb;
struct X : Bs { Sn s; El e[2]; X() : s() { g_ma.Register(&g_ta, 0x80, 2, 1, 0, 0); g_mb.Register(&g_tb, 0x80, 2, 6, 0, 0); } };
static bool Check() { X* x = new X; bool ok = g_base == 1 && g_snd == 1 && g_el == 2 && g_reg == 2 && g_last == 6; delete x; return ok; }

int main()
{
    if (!Check()) { std::printf("00c5b1a0_TEST FAIL\n"); return 1; }
    std::printf("00c5b1a0_TEST PASS\n");
    return 0;
}
