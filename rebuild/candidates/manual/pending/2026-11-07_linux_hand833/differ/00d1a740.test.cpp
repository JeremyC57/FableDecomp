// Behaviour test for NScript::CQ_BountyHuntScript::CBH_Hostage2::Main (0x00D1A740): the second hostage at Greatwood Lake. Without the
// intro she is removed. In GreatwoodEntrance she records a teleport entry (BH_Hostage2 -> GreatwoodLake, M_BH_Hostage2Pos) and waits
// for GreatwoodLake with FishComplete. At the lake she faces the living gang leader; once AllLeaderChargeGruntsKilled and the leader is
// killed or escaped: CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO (Hero / Victim), Hostage2Safe, gold reward, the reward object (taken from her
// if she holds it) unless the hero has one, walk out through M_BH_Greatwood3Exit, renown, fade out, LakeComplete. A state model.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Hostage {
    bool Intro, Lake, LeaderKilled, LeaderEscaped, GruntsKilled, HeroHasReward, SheHasReward, Safe, LakeComplete;
    int Frames, TerminateAt, EndAt, Gold;
    bool T() const { return Frames >= TerminateAt; }
    void Main()
    {
        if (!Intro) { Log("RemoveThing"); return; }
        if (!Lake) return;
        for (;;) {
            if (!LeaderKilled) Log("face_leader");
            if (GruntsKilled && (LeaderKilled || LeaderEscaped) && !Safe) {
                Log("CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO[Hero,Victim]");
                Safe = true; Gold += 100;
                if (!HeroHasReward) {
                    if (SheHasReward) { Log("take_from_her"); SheHasReward = false; }
                    Log("give_reward"); HeroHasReward = true;
                }
                Log("exit_M_BH_Greatwood3Exit"); Log("renown"); Log("fade_out");
                LakeComplete = true;
            }
            ++Frames;
            if (T()) return;
            if (Frames == EndAt) { GruntsKilled = true; LeaderEscaped = true; }
        }
    }
};
int main()
{
    Hostage a = { false, false, false, false, false, false, false, false, false, 0, 9, -1, 0 };
    a.Main();
    bool ok = !std::strcmp(g_log, "RemoveThing;");
    g_log[0] = 0;
    Hostage b = { true, true, false, false, false, false, true, false, false, 0, 3, 1, 0 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "face_leader;face_leader;CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO[Hero,Victim];take_from_her;give_reward;exit_M_BH_Greatwood3Exit;renown;fade_out;face_leader;")
        && b.Safe && b.LakeComplete && b.Gold == 100 && !b.SheHasReward;
    g_log[0] = 0;
    Hostage c = { true, true, true, false, true, true, false, false, false, 0, 2, -1, 0 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "CS_BOUNTYHUNT_GREATLAKEVICTIM_OUTRO[Hero,Victim];exit_M_BH_Greatwood3Exit;renown;fade_out;") && c.LakeComplete;
    std::printf(ok ? "00d1a740_TEST PASS\n" : "00d1a740_TEST FAIL\n");
    return ok ? 0 : 1;
}
