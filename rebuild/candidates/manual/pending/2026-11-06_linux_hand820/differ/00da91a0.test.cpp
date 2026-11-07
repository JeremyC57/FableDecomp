// Behaviour test for NScript::CQ_MinionClifftopChaseScript::CMapKeeper::Main (0x00DA91A0): the intro cut-scene once (DoneIntro), the
// cage effect kept on him until two guarding minions are dead (re-created whenever it dies), the escort watch (ends when the escort is
// gone or the hero leaves it), the wait for the hero within 15 units, then the cage fade, MapKeeperSaidDialog, the outro, removal and
// MissionSucceeded. A state model, run with and without the intro.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Keeper {
    bool DoneIntro, MapKeeperSaidDialog, MissionSucceeded, Terminate;
    int Frames, Killed, CagesMade, EscortDeadAt, HeroNearAt;
    bool CageDead() { return Frames == 2 || CagesMade == 0; }
    void NewScriptFrame() { ++Frames; if (Frames == 3) Killed = 2; if (Frames >= 40) Terminate = true; }
    bool IsActiveThreadTerminating() { return Terminate; }
    void Main()
    {
        NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!DoneIntro) { Log("CS_MINIONCHASE_PIER"); DoneIntro = true; Log("timer"); }
        Log("barriers_on");
        if (CageDead()) { ++CagesMade; Log("cage"); }
        while (Killed < 2) {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (CageDead()) { ++CagesMade; Log("cage"); }
        }
        bool escort_gone = false;
        do {
            NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Frames >= EscortDeadAt) escort_gone = true;
        } while (!escort_gone);
        while (!(Frames >= HeroNearAt)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        Log("cage_fade"); Log("barriers_off");
        MapKeeperSaidDialog = true;
        Log("CS_MINIONCHASE_OUTRO"); Log("remove");
        MissionSucceeded = true;
        for (;;) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    }
};
int main()
{
    Keeper a = { false, false, false, false, 0, 0, 0, 5, 8 };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_MINIONCHASE_PIER;timer;barriers_on;cage;cage;cage_fade;barriers_off;CS_MINIONCHASE_OUTRO;remove;")
        && a.DoneIntro && a.MapKeeperSaidDialog && a.MissionSucceeded && a.CagesMade == 2;
    g_log[0] = 0;
    Keeper b = { true, false, false, false, 0, 0, 1, 4, 4 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "barriers_on;cage;cage_fade;barriers_off;CS_MINIONCHASE_OUTRO;remove;") && b.MissionSucceeded;
    g_log[0] = 0;
    Keeper c = { false, false, false, true, 0, 0, 0, 5, 8 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "") && !c.MissionSucceeded && c.Frames == 1;
    std::printf(ok ? "00da91a0_TEST PASS\n" : "00da91a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
