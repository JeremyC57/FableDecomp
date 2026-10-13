#include <cstdio>

long GFGetCurrentFrame() { return 777; }
struct TP { unsigned char pad[0x3c]; bool TargetNextThing; unsigned char p2[3]; long Method; long Frame;
    void Set(long m) { TargetNextThing = true; Method = m; Frame = GFGetCurrentFrame(); } };
int main()
{
    TP t; t.TargetNextThing = false; t.Set(3);
    if (!t.TargetNextThing || t.Method != 3 || t.Frame != 777) { std::printf("008023c0_TEST FAIL\n"); return 1; }
    std::printf("008023c0_TEST PASS\n");
    return 0;
}
