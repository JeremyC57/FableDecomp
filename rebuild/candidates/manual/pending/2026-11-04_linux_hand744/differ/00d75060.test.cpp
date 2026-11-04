// Behaviour test for NScript::CQ_HangingTreeGoodScript::FinalRound (0x00D75060): a standalone model of the final round -- the
// executioner is created when missing, four freedom fighters are spawned (each targeting the executioner or the hero by rand()%2),
// then the round holds until every fighter and the boss are dead; the executioner dying fails the mission with its reason text.
#include <cstdio>
#include <cstdlib>
#include <string>
static int g_frames = 0;
struct Round {
    bool ExecutionerAlive, BossAlive, MissionFailed; int Fighters, Created, TargetExec, TargetHero, KillFightersAt, KillBossAt, KillExecAt;
    std::string ReasonForFailing;
    void Run()
    {
        if (!ExecutionerAlive) { ExecutionerAlive = true; ++Created; }
        for (long n = 0; n < 4; ++n) {
            ++Fighters;
            switch (std::rand() % 2) { case 0: ++TargetExec; break; case 1: ++TargetHero; break; }
        }
        for (;;) {
            bool allDead = g_frames >= KillFightersAt;
            if (g_frames >= KillBossAt) BossAlive = false;
            if (allDead && !BossAlive) break;
            ++g_frames;
            if (g_frames >= KillExecAt) ExecutionerAlive = false;
            if (!ExecutionerAlive) { ReasonForFailing = "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED"; MissionFailed = true; return; }
        }
    }
};
int main()
{
    Round r = { false, true, false, 0, 0, 0, 0, 3, 5, 1000 };
    r.Run();
    bool ok = r.Created == 1 && r.Fighters == 4 && r.TargetExec + r.TargetHero == 4 && !r.MissionFailed && !r.BossAlive && g_frames == 5;
    g_frames = 0;
    Round f = { true, true, false, 0, 0, 0, 0, 100, 100, 2 };
    f.Run();
    ok = ok && f.Created == 0 && f.MissionFailed && f.ReasonForFailing == "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED" && g_frames == 2;
    std::printf(ok ? "00d75060_TEST PASS\n" : "00d75060_TEST FAIL\n");
    return ok ? 0 : 1;
}
