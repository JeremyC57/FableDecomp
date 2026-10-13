#include <cstdio>

struct Def { unsigned char pad[0x25]; bool Can; };
struct Tc { unsigned char p[0x10]; float Cur; Def* def; unsigned char p2[0x14]; bool Over; unsigned char p3[0x1b]; float Res;
    long Is() { if (Over && def->Can && Cur >= Res) return 1; return 0; } };
int main()
{
    Def d; d.Can = true; Tc t; t.def = &d; t.Over = true; t.Cur = 2.0f; t.Res = 1.5f; if (t.Is() != 1) { std::printf("007c7890_TEST FAIL\n"); return 1; }
    t.Cur = 1.0f; if (t.Is() != 0) { std::printf("007c7890_TEST FAIL\n"); return 1; }
    t.Cur = 2.0f; d.Can = false; if (t.Is() != 0) { std::printf("007c7890_TEST FAIL\n"); return 1; }
    std::printf("007c7890_TEST PASS\n");
    return 0;
}