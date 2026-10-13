#include <cstdio>

long g_FramesPerSecond = 15; static long g_frame = 0;
long GFGetCurrentFrame() { return g_frame; }
struct Def { unsigned char pad[0x48]; long Seconds; };
struct W { unsigned char pad[0xc]; Def* def; unsigned char p2[0x74]; long Last;
    bool Wants() { Def* d = def; return GFGetCurrentFrame() - Last > d->Seconds * g_FramesPerSecond; } };
int main()
{
    Def d; d.Seconds = 2; W w; w.def = &d; w.Last = 100; g_frame = 130; if (w.Wants()) { std::printf("006ceef0_TEST FAIL\n"); return 1; }
    g_frame = 131; if (!w.Wants()) { std::printf("006ceef0_TEST FAIL\n"); return 1; }
    std::printf("006ceef0_TEST PASS\n");
    return 0;
}