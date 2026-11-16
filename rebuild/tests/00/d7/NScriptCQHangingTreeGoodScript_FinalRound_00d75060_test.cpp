// Behaviour test for NScript::CQ_HangingTreeGoodScript::FinalRound (0x00D75060): a standalone model of the final round with the
// terminate checks in source order -- create the executioner when he is missing, ally him with the hero and every guard, walk him to
// the prisoner, spawn four freedom fighters (`do { ... } while (++n < 4)`, each targeting the executioner or the hero by rand()%2), then
// hold until every fighter and the boss are dead: the executioner dying fails the mission with its reason text; an attack (not by the
// hero) makes him fight the nearest fighter at combat ability 20 until one of them dies, then restores 100 and clears the flag.
// A terminate request at any check returns at once (nothing after that check runs).
#include <cstdio>
#include <cstdlib>
#include <string>
struct Round {
    // inputs
    bool ExecutionerAlive, BossAlive; int Guards, ApproachFrames, KillFightersAt, KillBossAt, KillExecAt, AttackAt, NearestDiesAfter, TermAt;
    // outputs
    bool MissionFailed, ExecutionerBeenAttacked; int Created, Allied, Fighters, TargetExec, TargetHero, Frames, Checks, CombatSet20, CombatSet100;
    std::string ReasonForFailing;
    bool T() { return ++Checks == TermAt; }
    void Frame() { ++Frames; if (Frames == AttackAt) ExecutionerBeenAttacked = true; if (Frames >= KillExecAt) ExecutionerAlive = false; if (Frames >= KillBossAt) BossAlive = false; }
    void Run()
    {
        if (!ExecutionerAlive) {
            if (T()) return;
            ExecutionerAlive = true; ++Created;
        }
        for (int i = 0; i < Guards; ++i) {
            if (T()) return;
            Allied += 2;
        }
        if (T()) return;
        int walked = 0;
        while (true) {
            if (walked >= ApproachFrames) {
                if (T()) return;
                break;
            }
            ++walked;
            do { if (T()) return; } while (0);
        }
        long n = 0;
        do {
            do { if (T()) return; } while (0);
            ++Fighters;
            switch (std::rand() % 2) { case 0: ++TargetExec; break; case 1: ++TargetHero; break; }
        } while (++n < 4);
        if (T()) return;
        while (!(Frames >= KillFightersAt) || BossAlive) {
            Frame();
            if (T()) return;
            if (!ExecutionerAlive) {
                if (T()) return;
                ReasonForFailing = "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED";
                MissionFailed = true;
                return;
            }
            if (ExecutionerBeenAttacked) {
                if (T()) return;
                ++CombatSet20;
                int left = NearestDiesAfter;
                while (ExecutionerAlive && left > 0) {
                    Frame(); --left;
                    if (T()) return;
                }
                if (T()) return;
                ++CombatSet100;
                ExecutionerBeenAttacked = false;
            }
        }
        if (T()) return;
    }
};
static Round Make(bool execAlive, int termAt)
{
    Round r = { execAlive, true, 2, 3, 3, 5, 1000, -1, 0, termAt, false, false, 0, 0, 0, 0, 0, 0, 0, 0, 0, std::string() };
    return r;
}
int main()
{
    bool ok = true;
    Round a = Make(false, -1); a.Run();          // normal round: executioner created, 4 fighters, holds 5 frames
    ok = ok && a.Created == 1 && a.Allied == 4 && a.Fighters == 4 && a.TargetExec + a.TargetHero == 4 && !a.MissionFailed && !a.BossAlive && a.Frames == 5;
    Round b = Make(true, 1000); b.KillExecAt = 2; b.Run();   // executioner killed on frame 2: mission failed
    ok = ok && b.Created == 0 && b.MissionFailed && b.ReasonForFailing == "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED" && b.Frames == 2;
    // terminate at the 2nd spawn check: checks = guards 2 + post-guard 1 + approach (3 in-loop + 1 post) + spawn 2 = 9
    Round c = Make(true, 9); c.Run();
    ok = ok && c.Fighters == 1 && c.Frames == 0 && c.Allied == 4;
    Round d = Make(false, 1); d.Run();           // terminate before the executioner is created
    ok = ok && d.Created == 0 && d.Fighters == 0 && d.Allied == 0;
    Round e = Make(true, -1); e.AttackAt = 1; e.NearestDiesAfter = 2; e.KillFightersAt = 1; e.KillBossAt = 1; e.Run();   // attacked: fight back once
    ok = ok && e.CombatSet20 == 1 && e.CombatSet100 == 1 && !e.ExecutionerBeenAttacked && e.Frames == 3 && !e.MissionFailed;
    std::printf(ok ? "00d75060_TEST PASS\n" : "00d75060_TEST FAIL\n");
    return ok ? 0 : 1;
}