// Behaviour test for NScript::CQ_BountyHuntScript::CBH_GangDeputy1::Main (0x00D17F20): the first bandit deputy. Without the bounty
// hunt intro he is removed. In GreatwoodEntrance he walks to M_BH_FadeOut, records a teleport entry (BH_GangDeputy1 -> Fisherman,
// M_BH_Deputy1Pos), fades out and waits for the Fisherman region. In Fisherman: if the hostage is dead he walks out through
// M_BH_FishermanExit and vanishes (Deputy1Escaped); the first time the hero comes near / sees him / hits him he says
// TEXT_QST_017_DEPUTY1, sets Deputy1IntroComplete, spawns his grunts once (M_BH_GruntSpawnFisher) and the
// WatchForDeputy1GruntsKilled thread; then he attacks the hero. A state model of the control flow.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Deputy {
    bool IntroComplete, Entrance, Fisherman, HostageDead, HeroNear, Deputy1IntroComplete, SpawnGrunts, Escaped;
    int Frames, TerminateAt;
    bool T() const { return Frames >= TerminateAt; }
    void Frame() { ++Frames; }
    void Main()
    {
        if (!IntroComplete) { Log("RemoveThing"); return; }
        if (Entrance) {
            Log("walk_M_BH_FadeOut"); Log("teleport:BH_GangDeputy1>Fisherman@M_BH_Deputy1Pos"); Log("fade_out");
            while (!Fisherman) { Frame(); if (T()) return; Fisherman = Frames > 2; }
            return;
        }
        if (!Fisherman) return;
        Log("ally_BH_Hostage1");
        for (;;) {
            if (HostageDead) { Log("walk_M_BH_FishermanExit"); Escaped = true; Log("fade_out"); return; }
            if (!Deputy1IntroComplete) {
                while (!HeroNear && !HostageDead) { Frame(); if (T()) return; HeroNear = Frames > 1; }
                Log("TEXT_QST_017_DEPUTY1");
                Deputy1IntroComplete = true;
                if (!SpawnGrunts) { Log("grunts:M_BH_GruntSpawnFisher"); SpawnGrunts = true; }
                Log("spawn:ParentClass.WatchForDeputy1GruntsKilled");
            }
            Log("attack_hero");
            Frame();
            if (T()) return;
            HostageDead = true;
        }
    }
};
int main()
{
    Deputy a = { false, false, false, false, false, false, false, false, 0, 50 };
    a.Main();
    bool ok = !std::strcmp(g_log, "RemoveThing;");
    g_log[0] = 0;
    Deputy b = { true, true, false, false, false, false, false, false, 0, 50 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "walk_M_BH_FadeOut;teleport:BH_GangDeputy1>Fisherman@M_BH_Deputy1Pos;fade_out;");
    g_log[0] = 0;
    Deputy c = { true, false, true, false, false, false, false, false, 0, 50 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "ally_BH_Hostage1;TEXT_QST_017_DEPUTY1;grunts:M_BH_GruntSpawnFisher;spawn:ParentClass.WatchForDeputy1GruntsKilled;attack_hero;walk_M_BH_FishermanExit;fade_out;")
        && c.Deputy1IntroComplete && c.SpawnGrunts && c.Escaped;
    std::printf(ok ? "00d17f20_TEST PASS\n" : "00d17f20_TEST FAIL\n");
    return ok ? 0 : 1;
}