#include <cstdio>

long g_FramesPerSecond = 15;
struct Tc { unsigned char p[0xc]; long Will; float Cur; unsigned char p2[0xc]; long Last; unsigned char p3[0x24]; float Res;
    void Set(float s) { long f = (long)(g_FramesPerSecond * s); Last = f; if (Cur >= Res) Will = f; } };
int main()
{
    Tc t; t.Will = -1; t.Cur = 0.5f; t.Res = 1.0f; t.Set(2.0f); if (t.Last != 30 || t.Will != -1) { std::printf("007c7810_TEST FAIL\n"); return 1; }
    t.Cur = 1.0f; t.Set(1.0f); if (t.Last != 15 || t.Will != 15) { std::printf("007c7810_TEST FAIL\n"); return 1; }
    std::printf("007c7810_TEST PASS\n");
    return 0;
}
