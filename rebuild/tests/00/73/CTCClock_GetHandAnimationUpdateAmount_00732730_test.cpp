#include <cstdio>
#include <cmath>
struct CTimeOfDay { float Time; };
struct CWorldTimeComponent { CTimeOfDay* TimeOfDay; };
static CTimeOfDay g_tod;
static CWorldTimeComponent g_component = { &g_tod };
static CWorldTimeComponent* GetWorldTimeComponent() { return &g_component; }
struct CClockDef { long HandType; };
struct CTCClock {
    CClockDef* PDef; float LastTimeUpdated;
    float GetHandAnimationUpdateAmount()
    {
        float time = GetWorldTimeComponent()->TimeOfDay->Time;
        float delta;
        if (time > LastTimeUpdated) delta = time - LastTimeUpdated;
        else delta = time + (1.0f - LastTimeUpdated);
        long type = PDef->HandType;
        if (type == 2) return delta * 0.6666667f;
        if (type == 1) return delta * 8.0f;
        return 0.0f;
    }
};
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    CClockDef def = { 1 }; CTCClock clock; clock.PDef = &def; clock.LastTimeUpdated = 0.25f;
    g_tod.Time = 0.5f;
    if (!near(clock.GetHandAnimationUpdateAmount(), 2.0f)) { std::printf("00732730_TEST FAIL\n"); return 1; }
    def.HandType = 2; g_tod.Time = 0.125f;                     /* wrapped: 0.125 + 0.75 = 0.875 */
    if (!near(clock.GetHandAnimationUpdateAmount(), 0.875f * 0.6666667f)) { std::printf("00732730_TEST FAIL\n"); return 1; }
    def.HandType = 0;
    if (clock.GetHandAnimationUpdateAmount() != 0.0f) { std::printf("00732730_TEST FAIL\n"); return 1; }
    std::printf("00732730_TEST PASS\n");
    return 0;
}