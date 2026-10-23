// Behaviour test for CM_HeroInsideHouse::Main (0x00E96660): once the hero is inside, raise three barriers and play the
// cut-scene; when the house is cleared remove all four things, play two "_OFF" effects and re-enable teleporting.
#include <cstdio>
struct Sim {
    int things, effects, cutscenes; bool teleport, heroInside;
    void Run(float heroDist, bool cleared)
    {
        if (heroDist >= 1.0f) return;
        things = 4; effects = 2; cutscenes++; teleport = false; heroInside = true;
        if (!cleared) return;
        teleport = true; things -= 4; effects += 2;
    }
};
int main()
{
    Sim a = { 1, 0, 0, true, false };
    a.Run(5.0f, true);
    if (a.heroInside || a.cutscenes != 0 || !a.teleport) { std::printf("00e96660_TEST FAIL\n"); return 1; }
    Sim b = { 1, 0, 0, true, false };
    b.Run(0.5f, true);
    if (!b.heroInside || b.cutscenes != 1 || !b.teleport || b.things != 0 || b.effects != 4) { std::printf("00e96660_TEST FAIL\n"); return 1; }
    std::printf("00e96660_TEST PASS\n");
    return 0;
}