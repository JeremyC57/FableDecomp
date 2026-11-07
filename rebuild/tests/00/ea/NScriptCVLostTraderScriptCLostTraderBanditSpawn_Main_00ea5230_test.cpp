// Behaviour test for NScript::CV_LostTraderScript::CLostTraderBanditSpawn::Main (0x00EA5230): four bandit spawns in order -- the first
// waited to death, the second (set on the hero) only once Me is off camera and the hero over 15 units away, the third after the parent's
// ReachedTrader flag and the same off-camera wait, the last once the hero is within 30 units, then Me is removed. A state model run to
// completion and with an early termination.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Spawner {
    bool ReachedTrader, Terminate;
    int Frames, Spawned, DieAt, OffCameraAt, ReachedAt, NearAt, TerminateAt;
    void NewScriptFrame() { ++Frames; if (Frames >= ReachedAt) ReachedTrader = true; if (Frames >= TerminateAt) Terminate = true; }
    bool IsActiveThreadTerminating() { return Terminate; }
    int CreateCreature() { ++Spawned; Log("spawn"); return Frames + DieAt; }
    bool Alive(int deathFrame) { return Frames < deathFrame; }
    bool OnScreenOrNear() { return Frames < OffCameraAt; }
    bool HeroWithin30() { return Frames >= NearAt; }
    void Main()
    {
        int bandit = CreateCreature();
        while (Alive(bandit)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (OnScreenOrNear()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        int attacker = CreateCreature(); Log("target_hero");
        while (Alive(attacker)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (!ReachedTrader) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (OnScreenOrNear()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        attacker = CreateCreature(); Log("target_hero");
        while (Alive(attacker)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        while (!HeroWithin30()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        attacker = CreateCreature();
        Log("remove_me");
    }
};
int main()
{
    Spawner a = { false, false, 0, 0, 3, 5, 12, 20, 1000 };
    a.Main();
    bool ok = !std::strcmp(g_log, "spawn;spawn;target_hero;spawn;target_hero;spawn;remove_me;") && a.Spawned == 4 && a.ReachedTrader
        && a.Frames == 20;
    g_log[0] = 0;
    Spawner b = { false, false, 0, 0, 3, 5, 12, 20, 7 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "spawn;spawn;target_hero;") && b.Spawned == 2 && b.Frames == 7;
    g_log[0] = 0;
    Spawner c = { true, true, 0, 0, 3, 5, 12, 20, 1 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "spawn;") && c.Frames == 1;
    std::printf(ok ? "00ea5230_TEST PASS\n" : "00ea5230_TEST FAIL\n");
    return ok ? 0 : 1;
}