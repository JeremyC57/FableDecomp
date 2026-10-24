// Behaviour test for 008f8030 (LHNetworkLib::LTimer::MSeconds): elapsed real ticks scaled by SpeedUpFactor plus the timer time at the last adjust, truncated to unsigned long; tick wrap-around stays unsigned.
#include <cstdio>
typedef unsigned int TULONG;                   // the retail 32-bit unsigned long (the g++ host's is 64-bit)
static TULONG g_tickCount = 0;
static TULONG GetTickCount() { return g_tickCount; }   // stands in for the KERNEL32 import
namespace LHNetworkLib {
class LTimer {
public:
    char          buf[256];
    TULONG SystemClockAtLastAdjust;
    TULONG TimerTimeAtLastAdjust;
    float         SpeedUpFactor;
    float         SaveSpeedUpFactor;
    TULONG MSeconds();
};
TULONG LTimer::MSeconds()
{
    return (TULONG)((float)(GetTickCount() - SystemClockAtLastAdjust) * SpeedUpFactor + (float)TimerTimeAtLastAdjust);
}
}
int main()
{
    LHNetworkLib::LTimer timer;
    timer.SystemClockAtLastAdjust = 1000;
    timer.TimerTimeAtLastAdjust = 500;
    timer.SpeedUpFactor = 2.0f;
    timer.SaveSpeedUpFactor = 0.0f;
    g_tickCount = 1250;
    if (timer.MSeconds() != 1000) { std::printf("008f8030_TEST FAIL\n"); return 1; }   // 250 * 2 + 500
    timer.SpeedUpFactor = 0.5f;
    if (timer.MSeconds() != 625) { std::printf("008f8030_TEST FAIL\n"); return 1; }    // 250 * 0.5 + 500
    timer.SystemClockAtLastAdjust = 0xFFFFFF00U;                                      // the tick counter wrapped
    g_tickCount = 0x100;
    timer.SpeedUpFactor = 1.0f;
    timer.TimerTimeAtLastAdjust = 0;
    if (timer.MSeconds() != 0x200) { std::printf("008f8030_TEST FAIL\n"); return 1; }
    std::printf("008f8030_TEST PASS\n");
    return 0;
}