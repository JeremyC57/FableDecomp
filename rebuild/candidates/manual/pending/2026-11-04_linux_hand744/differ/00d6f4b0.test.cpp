// Behaviour test for NScript::CQ_HangingTreeGoodScript::Main (0x00D6F4B0): a standalone model -- wait for BowerstoneJail, ten
// entity-script bindings, the DoMission thread, wait for success, failure or the party's defeat; success fades out, pays the
// prisoner-kill gold/renown rewards when the hero killed the prisoner, gives the good morality, completes, drops the evil quest's
// card, removes the party and cleans up; otherwise fades out, fails with the reason, removes the party and the dregs; then fade
// in, re-enable the four creature-generator areas and deactivate the quest.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static int g_frames = 0, g_endAt = 100000, g_jailAt = 2, g_doneAt = 5, g_mode = 0;
static std::vector<std::string> g_log;
struct Game {
    bool *succ, *fail, *defeated;
    void NewScriptFrame() { ++g_frames; if (g_frames >= g_doneAt) { if (g_mode == 0) *succ = true; else if (g_mode == 1) *fail = true; else *defeated = true; } }
    bool IsLevelLoaded(const char* n) { return !std::strcmp(n, "BowerstoneJail") && g_frames >= g_jailAt; }
    void Log(const std::string& s) { g_log.push_back(s); }
};
struct Script {
    Game* G; bool MissionSucceeded, MissionFailed, PartyDefeated, HeroKilledPrisoner; int Bindings, Threads; std::string ReasonForFailing;
    bool Term() { return g_frames >= g_endAt; }
    void Main()
    {
        while (!G->IsLevelLoaded("BowerstoneJail")) { G->NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        Bindings += 10; Threads += 1;
        while (!MissionSucceeded && !MissionFailed && !PartyDefeated) { G->NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        if (MissionSucceeded) {
            G->Log("fadeout");
            if (HeroKilledPrisoner) { G->Log("gold"); G->Log("renown"); G->NewScriptFrame(); if (Term()) return; }
            G->Log("morality"); G->Log("completed"); G->Log("remove:Q_HangingTreeEvil"); G->Log("removeparty"); G->Log("cleanup");
        } else {
            G->Log("fadeout"); G->Log("failed:" + ReasonForFailing); G->Log("removeparty"); G->Log("dregs");
        }
        G->Log("fadein");
        G->Log("gen:BowerstoneJail"); G->Log("gen:GibbetWoods"); G->Log("gen:DesertedFarm"); G->Log("gen:ExecutionTree");
        G->Log("deactivate");
    }
};
static Script Fresh(Game& g) { Script s; s.G = &g; s.MissionSucceeded = s.MissionFailed = s.PartyDefeated = s.HeroKilledPrisoner = false; s.Bindings = s.Threads = 0; return s; }
int main()
{
    Game g; bool ok = true;
    Script a = Fresh(g); g.succ = &a.MissionSucceeded; g.fail = &a.MissionFailed; g.defeated = &a.PartyDefeated; a.HeroKilledPrisoner = true; a.Main();
    ok = ok && a.Bindings == 10 && a.Threads == 1 && g_log.size() == 14 && g_log[1] == "gold" && g_log[5] == "remove:Q_HangingTreeEvil";
    g_log.clear(); g_frames = 0; g_mode = 2;
    Script b = Fresh(g); g.succ = &b.MissionSucceeded; g.fail = &b.MissionFailed; g.defeated = &b.PartyDefeated; b.ReasonForFailing = "TEXT_Y"; b.Main();
    ok = ok && g_log.size() == 10 && g_log[1] == "failed:TEXT_Y" && g_log[3] == "dregs" && g_log.back() == "deactivate";
    g_log.clear(); g_frames = 0; g_mode = 0; g_endAt = 1;
    Script c = Fresh(g); g.succ = &c.MissionSucceeded; c.Main();
    ok = ok && g_log.empty() && c.Bindings == 0;
    std::printf(ok ? "00d6f4b0_TEST PASS\n" : "00d6f4b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
