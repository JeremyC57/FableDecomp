// Behaviour test for NScript::CQ_MinionClifftopChaseScript::Main (0x00DA34B0): bind the twenty entity scripts (all flag 1), post them,
// set the objective (TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_01, BowerstoneJail, BowerstoneSlums), wait one script frame per poll
// until BowerstoneJail is loaded (returning at once when the thread terminates), check the terminate flag again, override the music
// with the danger set (24) and spawn ProcessMission / WatchForTermination. A log model run three ways.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Quest {
    int Bindings, Flags, PollsUntilLoaded, Frames, TerminateAfterFrames;
    bool IsRegionLoaded(const char* r) { if (std::strcmp(r, "BowerstoneJail")) return false; return PollsUntilLoaded-- <= 0; }
    void NewScriptFrame() { ++Frames; Log("frame"); }
    bool IsActiveThreadTerminating() { return TerminateAfterFrames >= 0 && Frames >= TerminateAfterFrames; }
    void Bind(long flag) { ++Bindings; Flags += flag; }
    void Main()
    {
        for (int i = 0; i < 20; ++i) Bind(1);
        Log("post_script"); Log("post_game");
        Log("TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_01"); Log("BowerstoneJail"); Log("BowerstoneSlums");
        while (!IsRegionLoaded("BowerstoneJail")) {
            NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Log("music24");
        Log("ProcessMission"); Log("WatchForTermination");
    }
};
static bool Run(int polls, int term, const char* expect)
{
    g_log[0] = 0;
    Quest q = { 0, 0, polls, 0, term };
    q.Main();
    return q.Bindings == 20 && q.Flags == 20 && !std::strcmp(g_log, expect);
}
int main()
{
    const char* head = "post_script;post_game;TEXT_QUEST_MINION_CLIFFTOP_CHASE_OBJECTIVE_01;BowerstoneJail;BowerstoneSlums;";
    char full[512], waited[512], killed[512];
    std::sprintf(full, "%smusic24;ProcessMission;WatchForTermination;", head);
    std::sprintf(waited, "%sframe;frame;music24;ProcessMission;WatchForTermination;", head);
    std::sprintf(killed, "%sframe;", head);
    bool ok = Run(0, -1, full) && Run(2, -1, waited) && Run(5, 1, killed);
    std::printf(ok ? "00da34b0_TEST PASS\n" : "00da34b0_TEST FAIL\n");
    return ok ? 0 : 1;
}