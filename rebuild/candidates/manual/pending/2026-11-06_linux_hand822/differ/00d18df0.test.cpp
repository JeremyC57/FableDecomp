// Behaviour test for NScript::CQ_BountyHuntScript::CBH_GangDeputy2::Main (0x00D18DF0): removed when the intro is not done; after the
// bandit cut-scene, in the entrance before the fishing he records a lake teleport and fades; after the fishing he wanders, spawns his
// grunts once when fewer than four exist (plus the watch thread), waits to be provoked, follows the hero until close or hit while
// healthy, gives the intro speech + objective once (Deputy2IntroComplete clear) and attacks. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Deputy {
    bool IntroComplete, CutsceneDone, Entrance, FishComplete, SpawnGrunts, IntroDone2, Terminate;
    int Frames, Grunts, Teleports, ProvokeAt, CloseAt;
    void NewScriptFrame() { ++Frames; if (Frames >= 12) Terminate = true; }
    bool IsActiveThreadTerminating() { return Terminate; }
    void Main()
    {
        if (!IntroComplete) { Log("remove"); return; }
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!CutsceneDone) return;
        if (!Entrance) return;
        if (!FishComplete) { Log("walk_fadeout"); ++Teleports; Log("fade"); return; }
        Log("wander");
        if (!SpawnGrunts) {
            if (Grunts < 4) { Log("spawn_grunts"); Log("watch_thread"); }
            SpawnGrunts = true;
        }
        while (!(Frames >= ProvokeAt)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        Log("follow");
        while (!(Frames >= CloseAt)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (!IntroDone2) { Log("TEXT_QST_017_DEPUTY2"); Log("objective04"); }
        Log("attack");
        for (;;) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    }
};
int main()
{
    Deputy a = { true, true, true, true, false, false, false, 0, 2, 0, 3, 5 };
    a.Main();
    bool ok = !std::strcmp(g_log, "wander;spawn_grunts;watch_thread;follow;TEXT_QST_017_DEPUTY2;objective04;attack;") && a.SpawnGrunts && a.Frames == 12;
    g_log[0] = 0;
    Deputy b = { true, true, true, false, false, false, false, 0, 2, 0, 3, 5 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "walk_fadeout;fade;") && b.Teleports == 1;
    g_log[0] = 0;
    Deputy c = { false, true, true, true, false, false, false, 0, 2, 0, 3, 5 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "remove;") && c.Frames == 0;
    g_log[0] = 0;
    Deputy d = { true, true, true, true, false, true, false, 0, 6, 0, 2, 2 };
    d.Main();
    ok = ok && !std::strcmp(g_log, "wander;follow;attack;") && d.SpawnGrunts;
    std::printf(ok ? "00d18df0_TEST PASS\n" : "00d18df0_TEST FAIL\n");
    return ok ? 0 : 1;
}
