// Behaviour test for NScript::CQ_HangingTreeEvilScript::Main (0x00D69480): a standalone model -- quest card objective 1, wait for
// BowerstoneJail, a 1000-tick mission timer, eight entity-script bindings, the DoMission / BanditYells threads, then success
// (clean up, complete, evil morality, remove the good quest's card) or failure (fade, clean up, fail with the reason), and in
// both cases fade in, re-enable the four creature-generator areas and deactivate the quest.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static int g_frames = 0, g_endAt = 100000, g_jailAt = 2, g_doneAt = 5;
static std::vector<std::string> g_log;
struct Game {
    bool* succ; bool* fail; bool failInstead;
    void NewScriptFrame() { ++g_frames; if (g_frames >= g_doneAt) { if (failInstead) *fail = true; else *succ = true; } }
    bool IsLevelLoaded(const char* n) { return !std::strcmp(n, "BowerstoneJail") && g_frames >= g_jailAt; }
    void Log(const std::string& s) { g_log.push_back(s); }
};
struct Script {
    Game* G; bool MissionSucceeded, MissionFailed; long Timer; int Bindings, Threads, CleanUps; std::string ReasonForFailing;
    bool Term() { return g_frames >= g_endAt; }
    void Main()
    {
        G->Log("objective");
        while (!G->IsLevelLoaded("BowerstoneJail")) { G->NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        Timer = 1000;
        G->Log("objective");
        const char* names[8] = { "HTE_AmbushPoint", "HTE_AssaultGuard", "HTE_FreedomFighter", "HTE_Audience", "HTE_Prisoner", "HTE_Executioner", "GUARD1", "GUARD2" };
        for (int i = 0; i < 8; ++i) if (names[i][0]) ++Bindings;
        Threads += 2;                                   // DoMission, BanditYells
        while (!MissionSucceeded && !MissionFailed) { G->NewScriptFrame(); if (Term()) return; }
        if (Term()) return;
        if (MissionSucceeded) { ++CleanUps; G->Log("completed"); G->Log("morality"); G->Log("remove:Q_HangingTreeGood"); }
        else { G->Log("fadeout"); ++CleanUps; G->Log("failed:" + ReasonForFailing); }
        G->Log("fadein");
        G->Log("gen:BowerstoneJail"); G->Log("gen:GibbetWoods"); G->Log("gen:DesertedFarm"); G->Log("gen:ExecutionTree");
        G->Log("deactivate");
    }
};
int main()
{
    Game g; Script s; s.G = &g; s.MissionSucceeded = s.MissionFailed = false; s.Timer = 0; s.Bindings = s.Threads = s.CleanUps = 0;
    g.succ = &s.MissionSucceeded; g.fail = &s.MissionFailed; g.failInstead = false;
    s.Main();
    bool ok = s.Timer == 1000 && s.Bindings == 8 && s.Threads == 2 && s.CleanUps == 1 && g_log.size() == 11 && g_log[2] == "completed"
           && g_log[4] == "remove:Q_HangingTreeGood" && g_log.back() == "deactivate";
    g_log.clear(); g_frames = 0; Script t = s; t.MissionSucceeded = false; g.succ = &t.MissionSucceeded; g.fail = &t.MissionFailed;
    g.failInstead = true; t.ReasonForFailing = "TEXT_X"; t.Main();
    ok = ok && t.CleanUps == 2 && g_log[2] == "fadeout" && g_log[3] == "failed:TEXT_X" && g_log.size() == 10;
    g_log.clear(); g_frames = 0; g_endAt = 1; Script u = s; u.Main();  // terminating during the jail wait: nothing else runs
    ok = ok && g_log.size() == 1 && u.Bindings == 8;
    std::printf(ok ? "00d69480_TEST PASS\n" : "00d69480_TEST FAIL\n");
    return ok ? 0 : 1;
}
