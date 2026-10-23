// Behaviour test for CTCCreatureGroupBoastingCrowd::OnHeroLeaveBoastingArea (0x007846B0): after a boast the crowd enters
// state 7 (good boast) or 8 and the clear frame is set 30..60 s ahead; without a boast it enters 9 while the quest
// manager still lists boasts (with the same delay), otherwise 0 (the clear frame is untouched).
#include <cstdio>
static long g_framesPerSecond = 30;
static long g_worldFrame = 1000;
static bool g_boastsListed = true;
static float g_random = 0.25f;
struct CTCCreatureGroupBoastingCrowd {
    bool HeroBoasted;
    bool HeroMadeGoodBoast;
    long FrameToClearRemainingOpinionSources;
    long LastState;
    void EnterCrowdState(long state) { LastState = state; }
    void OnHeroLeaveBoastingArea()
    {
        if (HeroBoasted) {
            long fps = g_framesPerSecond;
            float delay = (g_random + 0.5f) * fps * 60.0f;
            FrameToClearRemainingOpinionSources = (long)(g_worldFrame + delay);
            EnterCrowdState(HeroMadeGoodBoast ? 7 : 8);
            return;
        }
        if (g_boastsListed) {
            long fps = g_framesPerSecond;
            float delay = (g_random + 0.5f) * fps * 60.0f;
            FrameToClearRemainingOpinionSources = (long)(g_worldFrame + delay);
            EnterCrowdState(9);
        } else
            EnterCrowdState(0);
    }
};
static int fail() { std::printf("007846b0_TEST FAIL\n"); return 1; }
int main()
{
    CTCCreatureGroupBoastingCrowd c = { true, true, -1, -1 };
    c.OnHeroLeaveBoastingArea();
    if (c.LastState != 7 || c.FrameToClearRemainingOpinionSources != 1000 + 1350) return fail();
    c.HeroMadeGoodBoast = false;
    c.OnHeroLeaveBoastingArea();
    if (c.LastState != 8) return fail();
    c.HeroBoasted = false;
    g_random = 0.5f;
    c.OnHeroLeaveBoastingArea();
    if (c.LastState != 9 || c.FrameToClearRemainingOpinionSources != 1000 + 1800) return fail();
    g_boastsListed = false;
    c.FrameToClearRemainingOpinionSources = 5;
    c.OnHeroLeaveBoastingArea();
    if (c.LastState != 0 || c.FrameToClearRemainingOpinionSources != 5) return fail();
    std::printf("007846b0_TEST PASS\n");
    return 0;
}