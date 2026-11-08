// Behaviour test for NScript::CV_ExposeMayorScript::HeroFlashesLamp (0x00E76E90): a copy of the recursive watcher over a mock
// game with a per-frame lamp timeline and a 3-frame timer; checks that `flashes` toggles of the lamp within the timer windows
// recurse down to the flashes == 0 check (HeroFlashedLightsCorrectly when the hero is near EM_FlashLightPoint, else ResetLamp),
// that a timed-out window sets ResetLamp, and that the timers are registered / deregistered in balance.
#include <cstdio>
#include <vector>

struct MockGame {
    std::vector<int> lit; size_t frame; int timers; bool heroNear;
    bool IsHeroHandLampLit() { return frame < lit.size() ? lit[frame] != 0 : false; }
    void NewScriptFrame() { ++frame; }
    long RegisterTimer() { ++timers; return 1; }
    void DeregisterTimer(long) { --timers; }
    long GetHero() { return 1; }
    long GetThingWithScriptName(const char*) { return 2; }
};
static MockGame* pDSTGame;
struct CTimer {
    long start, length;
    CTimer() : start(0), length(0) { pDSTGame->RegisterTimer(); }
    ~CTimer() { pDSTGame->DeregisterTimer(1); }
    CTimer& operator=(long n) { start = (long)pDSTGame->frame; length = n; return *this; }
    long GetTime() const { return start + length - (long)pDSTGame->frame; }
};
static bool IsDistanceBetweenThingsUnder(long, long, float) { return pDSTGame->heroNear; }
struct Script {
    MockGame* Game; float MaxFlashDistFromStables; bool HeroFlashedLightsCorrectly, ResetLamp; int terminateAt, checks;
    bool IsActiveThreadTerminating() { return ++checks == terminateAt; }
    void HeroFlashesLamp(long flashes)
    {
        if (flashes == 0) {
            if (IsActiveThreadTerminating()) return;
            long point = Game->GetThingWithScriptName("EM_FlashLightPoint");
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), point, MaxFlashDistFromStables)) { if (IsActiveThreadTerminating()) return; HeroFlashedLightsCorrectly = true; }
            else { if (IsActiveThreadTerminating()) return; ResetLamp = true; }
        } else {
            CTimer timer; timer = 3;
            if (Game->IsHeroHandLampLit()) {
                if (IsActiveThreadTerminating()) return;
                while (timer.GetTime() > 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; if (!Game->IsHeroHandLampLit()) { if (IsActiveThreadTerminating()) return; HeroFlashesLamp(flashes - 1); } }
                if (IsActiveThreadTerminating()) return;
            } else {
                if (IsActiveThreadTerminating()) return;
                while (timer.GetTime() > 0) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; if (Game->IsHeroHandLampLit()) { if (IsActiveThreadTerminating()) return; HeroFlashesLamp(flashes - 1); } }
                if (IsActiveThreadTerminating()) return;
            }
            ResetLamp = true;
        }
    }
};
static bool Run(const int* lit, int n, bool near, long flashes, int terminateAt, bool expectCorrect, bool expectReset)
{
    MockGame g; g.lit.assign(lit, lit + n); g.frame = 0; g.timers = 0; g.heroNear = near; pDSTGame = &g;
    Script s; s.Game = &g; s.MaxFlashDistFromStables = 5.0f; s.HeroFlashedLightsCorrectly = false; s.ResetLamp = false; s.terminateAt = terminateAt; s.checks = 0;
    s.HeroFlashesLamp(flashes);
    return s.HeroFlashedLightsCorrectly == expectCorrect && s.ResetLamp == expectReset && g.timers == 0;
}
int main()
{
    int quick[] = { 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 };      // lit, off, on, off: two toggles inside the windows
    int slow[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };       // never toggles: the window times out
    if (!Run(quick, 10, true, 2, 0, true, true)) { std::printf("00e76e90_TEST FAIL\n"); return 1; }    // flashes done near the point (the outer levels still set ResetLamp)
    if (!Run(quick, 10, false, 2, 0, false, true)) { std::printf("00e76e90_TEST FAIL\n"); return 1; }  // flashes done away from the point
    if (!Run(slow, 10, true, 1, 0, false, true)) { std::printf("00e76e90_TEST FAIL\n"); return 1; }    // timed out: reset only
    if (!Run(quick, 10, true, 0, 0, true, false)) { std::printf("00e76e90_TEST FAIL\n"); return 1; }   // zero flashes: the position check alone
    if (!Run(quick, 10, true, 2, 3, false, false)) { std::printf("00e76e90_TEST FAIL\n"); return 1; }  // terminated in the first window: nothing set, timer released
    std::printf("00e76e90_TEST PASS\n");
    return 0;
}
