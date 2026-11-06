// Behaviour test for NScript::CQ_BountyHuntScript::CBH_GangLeader::Main (0x00D1B200): before the bounty-hunt intro the leader is
// removed; after the bandit cut-scene, in GreatwoodEntrance he runs to the fade-out marker, records a teleport to GreatwoodLake and
// fades out; in GreatwoodLake he plays the victim cut-scene and spawns the lake grunts once, then each frame: a hero hit charges
// (LEADER6 once, charge grunts, attacking hero); approach 1 / approach 2 give the demand / warning; after the warning he attacks the
// hostage (reward object + gold to her, charge grunts); once the hostage is dead he runs to the exit, escapes and fades out.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Leader {
    int Frames, TerminateAt, Region, HitMask, Approach1Mask, Approach2Mask, HostageDiesAt, ExitDistance, Teleports, GruntSpawns;
    bool IntroComplete, CutsceneDone, LeaderCutscene, SpawnedLakeGrunts, Charged, AttackingHero, Intro1, Intro2, AttackingHostage,
         HostageKilled, Escaped, HostageHasReward;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Main() {
        if (!IntroComplete) { if (T()) return; Log("remove"); return; }
        if (T()) return;
        while (!CutsceneDone) { NSF(); if (T()) return; }
        if (Region == 1) {
            Log("run_to_fade_out"); ++Teleports; Log("fade");
            for (;;) { NSF(); if (T()) return; }
        }
        if (Region != 2) return;
        if (!LeaderCutscene) { Log("CS_GREATLAKEVICTIM"); LeaderCutscene = true; }
        if (!SpawnedLakeGrunts) { ++GruntSpawns; SpawnedLakeGrunts = true; }
        for (; !T(); NSF()) {
            if (Frames >= HostageDiesAt) HostageKilled = true;
            if (Bit(HitMask) && !AttackingHero && !HostageKilled && !AttackingHostage) {
                if (!Charged) { Log("LEADER6"); Charged = true; }
                ++GruntSpawns; AttackingHero = true;
            }
            if (!AttackingHero && !Intro1 && Bit(Approach1Mask)) {
                if (!HostageKilled) Log("DEMAND_2");
                Intro1 = true;
            } else if (!AttackingHero && !Intro2 && Bit(Approach2Mask)) {
                if (!HostageKilled) Log("WARNING_3");
                Intro2 = true;
            }
            if (Intro2 && !HostageKilled && !AttackingHostage && !AttackingHero) {
                HostageHasReward = true; Log("LEADER7"); ++GruntSpawns; AttackingHostage = true;
            }
            if (HostageKilled) {
                while (ExitDistance >= 2) { --ExitDistance; NSF(); if (T()) return; }
                Escaped = true; Log("fade");
                for (;;) { NSF(); if (T()) return; }
            }
        }
    }
};
int main()
{
    Leader a = { 0, 30, 2, 0, (1 << 2), (1 << 4), 8, 3, 0, 0,
                 true, true, false, false, false, false, false, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "CS_GREATLAKEVICTIM;DEMAND_2;WARNING_3;LEADER7;fade;") && a.Escaped && a.GruntSpawns == 2
              && a.HostageHasReward && a.Frames == 30;
    g_log[0] = 0;
    Leader b = { 0, 12, 2, (1 << 1) | (1 << 3), (1 << 2), (1 << 4), 99, 3, 0, 0,
                 true, true, true, true, false, false, false, false, false, false, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "LEADER6;") && b.AttackingHero && b.GruntSpawns == 1 && !b.Intro1 && !b.Escaped;
    g_log[0] = 0;
    Leader c = { 0, 10, 1, 0, 0, 0, 99, 0, 0, 0,
                 true, true, false, false, false, false, false, false, false, false, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "run_to_fade_out;fade;") && c.Teleports == 1;
    g_log[0] = 0;
    Leader d = { 0, 10, 2, 0, 0, 0, 99, 0, 0, 0,
                 false, true, false, false, false, false, false, false, false, false, false, false };
    d.Main();
    ok = ok && !std::strcmp(g_log, "remove;") && d.Frames == 0;
    std::printf(ok ? "00d1b200_TEST PASS\n" : "00d1b200_TEST FAIL\n");
    return ok ? 0 : 1;
}
