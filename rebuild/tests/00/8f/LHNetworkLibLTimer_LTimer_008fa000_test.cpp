// Behaviour test for LHNetworkLib::LTimer::LTimer() (0x008FA000): unit speed-up factor, zero saved factor, the
// current tick count as the system clock, zero timer time, Stop() called twice.
#include <cstdio>
static unsigned long Ticks() { return 4242; }
namespace LHNetworkLib {
class LTimer {
public:
    char buf[256]; unsigned long SystemClockAtLastAdjust; unsigned long TimerTimeAtLastAdjust;
    float SpeedUpFactor; float SaveSpeedUpFactor;
    static int s_stops;
    LTimer() { SpeedUpFactor = 1.0f; SaveSpeedUpFactor = 0; SystemClockAtLastAdjust = Ticks(); TimerTimeAtLastAdjust = 0; Stop(); Stop(); }
    void Stop() { ++s_stops; }
};
int LTimer::s_stops = 0;
}
int main()
{
    LHNetworkLib::LTimer t;
    if (t.SpeedUpFactor != 1.0f || t.SaveSpeedUpFactor != 0 || t.SystemClockAtLastAdjust != 4242 || t.TimerTimeAtLastAdjust
        || LHNetworkLib::LTimer::s_stops != 2) { std::printf("008fa000_TEST FAIL\n"); return 1; }
    std::printf("008fa000_TEST PASS\n"); return 0;
}
