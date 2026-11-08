// Behaviour test for NScript::CQ_BountyHuntScript::CBH_Hostage1::Main (0x00D19BD0): the Fisherman (first hostage). Without the intro
// he is removed. In GreatwoodEntrance he is made unkillable, records a teleport entry (BH_Hostage1 -> Fisherman, M_BH_Hostage1Pos) and
// waits for the Fisherman region. In Fisherman: BOUNTY_HUNT_FISHERMAN_VICTIM with Hero / Victim / Bandit, then each frame he faces the
// living deputy; once Deputy1Killed and AllDeputy1GruntsKilled: CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO (Hero / Victim), FishComplete,
// objective 03 (GreatwoodLake / Greatwood), Hostage1Safe, walk out through M_BH_FishermanExit, fade out and renown. A state model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Hostage {
    bool Intro, Entrance, Fisherman, DeputyKilled, GruntsKilled, FishComplete, Safe;
    int Frames, TerminateAt, KillAt;
    bool T() const { return Frames >= TerminateAt; }
    void Main()
    {
        if (!Intro) { Log("RemoveThing"); return; }
        if (Entrance) {
            Log("unkillable"); Log("teleport:BH_Hostage1>Fisherman@M_BH_Hostage1Pos");
            while (!Fisherman) { ++Frames; if (T()) return; }
            return;
        }
        if (!Fisherman) return;
        Log("BOUNTY_HUNT_FISHERMAN_VICTIM[Hero,Victim,Bandit]");
        for (;;) {
            if (!DeputyKilled) Log("face_deputy");
            if (DeputyKilled && GruntsKilled && !Safe) {
                FishComplete = true;
                Log("CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO[Hero,Victim]");
                Log("TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_03");
                Safe = true;
                Log("exit_M_BH_FishermanExit"); Log("renown");
            }
            ++Frames;
            if (T()) return;
            if (Frames == KillAt) DeputyKilled = GruntsKilled = true;
        }
    }
};
int main()
{
    Hostage a = { false, false, false, false, false, false, false, 0, 9, -1 };
    a.Main();
    bool ok = !std::strcmp(g_log, "RemoveThing;");
    g_log[0] = 0;
    Hostage b = { true, false, true, false, false, false, false, 0, 4, 2 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "BOUNTY_HUNT_FISHERMAN_VICTIM[Hero,Victim,Bandit];face_deputy;face_deputy;CS_BOUNTYHUNT_FISHERMANVICTIM_OUTRO[Hero,Victim];TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_03;exit_M_BH_FishermanExit;renown;")
        && b.FishComplete && b.Safe;
    g_log[0] = 0;
    Hostage c = { true, true, false, false, false, false, false, 0, 3, -1 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "unkillable;teleport:BH_Hostage1>Fisherman@M_BH_Hostage1Pos;");
    std::printf(ok ? "00d19bd0_TEST PASS\n" : "00d19bd0_TEST FAIL\n");
    return ok ? 0 : 1;
}
