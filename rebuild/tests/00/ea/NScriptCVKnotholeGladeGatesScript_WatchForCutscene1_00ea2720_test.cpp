// behaviour test: CV_KnotholeGladeGatesScript::WatchForCutscene1 -- guard made unpushable/unkillable, wait for BalvsKilled, play the
// gates cutscene with HERO / CHIEF / LOOKOUT inside a paused movie, set SaidBefore / GatesOpened, grant experience + renown, remove the
// guard if still alive; an early terminate returns before the cutscene
#include <stdio.h>
#include <string.h>
static int g_frames, g_killAt, g_endAt, g_paused, g_pauses, g_exp, g_renown, g_removed, g_cutscene, g_mapEntries, g_pushable, g_killable;
static bool g_guardAlive;
struct Thing { int id; };
struct Script {
    bool MissionSucceeded, GatesOpened, SaidBefore, BalvsKilled, CutsceneStarting;
    bool IsActiveThreadTerminating() const { return g_frames >= g_endAt; }
    void NewScriptFrame() { ++g_frames; if (g_frames >= g_killAt) BalvsKilled = true; }
    void WatchForCutscene1();
};
void Script::WatchForCutscene1()
{
    Thing gate_guard = { 2 };
    g_pushable = 0; g_killable = 0;
    while (!BalvsKilled) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    CutsceneStarting = true;
    {
        const char* keys[3] = { "HERO", "CHIEF", "LOOKOUT" };
        for (int i = 0; i < 3; ++i) if (keys[i][0]) ++g_mapEntries;
        g_paused = 1; ++g_pauses;
        g_cutscene = 1;
        SaidBefore = true;
        GatesOpened = true;
        g_exp += 500; g_renown += 30;
        g_paused = 0;
    }
    if (g_guardAlive && gate_guard.id) ++g_removed;
}
static int Run(int killAt, int endAt, bool alive, Script& s)
{
    memset(&s, 0, sizeof(s));
    g_frames = 0; g_killAt = killAt; g_endAt = endAt; g_paused = g_pauses = g_exp = g_renown = g_removed = g_cutscene = g_mapEntries = 0;
    g_guardAlive = alive; g_pushable = g_killable = 1;
    s.WatchForCutscene1();
    return 0;
}
int main()
{
    Script s;
    Run(3, 100, true, s);
    if (!s.GatesOpened || !s.SaidBefore || !s.CutsceneStarting || g_cutscene != 1 || g_mapEntries != 3 || g_paused != 0 || g_pauses != 1
        || g_exp != 500 || g_renown != 30 || g_removed != 1 || g_pushable != 0 || g_killable != 0) { printf("00ea2720_TEST FAIL\n"); return 1; }
    Run(3, 100, false, s);
    if (g_removed != 0 || !s.GatesOpened) { printf("00ea2720_TEST FAIL\n"); return 1; }
    Run(50, 5, true, s);
    if (s.GatesOpened || s.CutsceneStarting || g_cutscene || g_exp) { printf("00ea2720_TEST FAIL\n"); return 1; }
    printf("00ea2720_TEST PASS\n");
    return 0;
}