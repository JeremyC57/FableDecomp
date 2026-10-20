// Behaviour test for CTCCreatureGroupBoastingCrowd::FrameUpdate (0x00784500).
#include <cstdio>

static long g_frame = 0;
static int g_killed = 0, g_cleared = 0;
static long g_entered = -1;
static bool g_boastsLeft = false;

struct Thing { bool dead; };
struct Crowd {
    Thing* hero;
    long CrowdState, CurrentEncouragementState;
    bool HeroMadeGoodBoast;
    long HeroOpinionSourcesPushed, FrameToClearRemainingOpinionSources, FrameToLeavePostBoastState;
    void EnterCrowdState(long s) { g_entered = s; }
    bool OffPodium() const { long s = CrowdState; return s == 0 || s == 1 || s == 7 || s == 8 || s == 9 || s == 10; }
    bool IsHeroAlive() const { return hero && !hero->dead; }
    void FrameUpdate()
    {
        if (!IsHeroAlive()) { ++g_killed; return; }
        if (HeroOpinionSourcesPushed > 0 && OffPodium() && FrameToClearRemainingOpinionSources > 0 &&
            g_frame > FrameToClearRemainingOpinionSources) {
            FrameToClearRemainingOpinionSources = 0;
            while (HeroOpinionSourcesPushed > 0) { ++g_cleared; --HeroOpinionSourcesPushed; }
            EnterCrowdState(0);
        }
        if ((CrowdState == 5 || CrowdState == 6) && g_frame > FrameToLeavePostBoastState) {
            FrameToLeavePostBoastState = 0;
            if (g_boastsLeft) EnterCrowdState(CurrentEncouragementState);
            else EnterCrowdState(HeroMadeGoodBoast ? 7 : 8);
        }
    }
};

int main()
{
    Thing h = { false };
    Crowd c = { 0, 0, 0, false, 0, 0, 0 };
    c.FrameUpdate();
    if (g_killed != 1) { std::printf("00784500_TEST FAIL\n"); return 1; }
    c.hero = &h; h.dead = true; c.FrameUpdate();
    if (g_killed != 2) { std::printf("00784500_TEST FAIL\n"); return 1; }
    h.dead = false;
    c.CrowdState = 7; c.HeroOpinionSourcesPushed = 3; c.FrameToClearRemainingOpinionSources = 10; g_frame = 11;
    c.FrameUpdate();
    if (g_cleared != 3 || c.HeroOpinionSourcesPushed != 0 || c.FrameToClearRemainingOpinionSources != 0 || g_entered != 0) {
        std::printf("00784500_TEST FAIL\n"); return 1;
    }
    g_entered = -1; c.CrowdState = 5; c.FrameToLeavePostBoastState = 20; g_frame = 21; c.HeroMadeGoodBoast = true;
    c.FrameUpdate();
    if (g_entered != 7 || c.FrameToLeavePostBoastState != 0) { std::printf("00784500_TEST FAIL\n"); return 1; }
    c.HeroMadeGoodBoast = false; c.FrameUpdate();
    if (g_entered != 8) { std::printf("00784500_TEST FAIL\n"); return 1; }
    g_boastsLeft = true; c.CurrentEncouragementState = 3; c.CrowdState = 6; c.FrameUpdate();
    if (g_entered != 3) { std::printf("00784500_TEST FAIL\n"); return 1; }
    std::printf("00784500_TEST PASS\n");
    return 0;
}
