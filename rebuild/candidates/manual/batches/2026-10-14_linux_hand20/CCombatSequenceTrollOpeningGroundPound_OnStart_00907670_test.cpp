#include <cstdio>
static long g_frame = 1000; static long __fastcall GFGetCurrentFrame() { return g_frame; } static long g_logicFramesPerSecond = 15;
struct CSeq { long WFNextHappen; void OnStart() { long frame = GFGetCurrentFrame(); WFNextHappen = g_logicFramesPerSecond * 15.0f + frame; } };
int main()
{
    CSeq s; s.OnStart();
    if (s.WFNextHappen != 1225) { std::printf("00907670_TEST FAIL\n"); return 1; }
    g_frame = 7; g_logicFramesPerSecond = 2; s.OnStart();
    if (s.WFNextHappen != 37) { std::printf("00907670_TEST FAIL\n"); return 1; }
    std::printf("00907670_TEST PASS\n");
    return 0;
}
