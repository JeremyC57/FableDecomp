#include <cstdio>
#include <cstring>
// behaviour test: CQ_WizardBattleScript::PhaseOne -- wait at the barrier, chant, remove the barrier, wait at the
// kick-off point, play the intro, then bind Maze and set the Hook Coast objective
static int g_frame = 0;
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Script {
    bool VillagersTurnOnHero; int heroPos; int convEnds; bool mazeBound;
    bool IsActiveThreadTerminating() { return g_frame > 100; }
    void Frame() { ++g_frame; ++heroPos; }
    bool NearBarrier() { return heroPos >= 5; }
    bool NearKickoff() { return heroPos >= 20; }
    void PhaseOne()
    {
        Log("marker");
        while (!NearBarrier()) { Frame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("chant"); convEnds = g_frame + 3;
        while (g_frame < convEnds) { Frame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("remove"); VillagersTurnOnHero = true;
        while (!NearKickoff()) { Frame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        Log("intro"); mazeBound = true; Log("objective");
    }
};
int main()
{
    Script s; s.VillagersTurnOnHero = false; s.heroPos = 0; s.convEnds = 0; s.mazeBound = false; g_log[0] = 0;
    s.PhaseOne();
    if (std::strcmp(g_log, "marker;chant;remove;intro;objective;") != 0 || !s.VillagersTurnOnHero || !s.mazeBound || g_frame != 20) {
        std::printf("00e1cfd0_TEST FAIL %s %d\n", g_log, g_frame); return 1;
    }
    std::printf("00e1cfd0_TEST PASS\n");
    return 0;
}