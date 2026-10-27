#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
struct Thing { int Id; };
struct Game { Thing Hero;
    void EntitySetAsAllowedToFollowHero(const Thing&, bool f) { LogI("follow", f); }
    void EntitySetAsScared(const Thing&, bool s) { LogI("scared", s); }
    void MiniMapAddMarker(const Thing&, const char* m) { LogS("marker", m); }
    Thing& GetHero() { return Hero; }
    void SetEntityAsRegionFollowing(const Thing& a, const Thing& b, bool f) { LogI("regionfollow", a.Id * 100 + b.Id * 10 + f); }
    void EntitySetAsAllowedToChangeRegionFollowingState(const Thing&, bool c) { LogI("change", c); }
};
struct Trader { Game* G; Thing Me; int stateBrain, YellForHelp, YellAtHero, RandomComment;
    void Init()
    {
        stateBrain = 1; YellForHelp = 0; YellAtHero = 0; RandomComment = 0;
        G->EntitySetAsAllowedToFollowHero(Me, true);
        G->EntitySetAsScared(Me, true);
        G->MiniMapAddMarker(Me, "HUD_ORB_QUEST_OPTIONAL");
        G->SetEntityAsRegionFollowing(G->GetHero(), Me, false);
        G->EntitySetAsAllowedToChangeRegionFollowingState(Me, false);
    }
};
int main()
{
    bool ok = true;
    Game g; g.Hero.Id = 1; Trader t; t.G = &g; t.Me.Id = 2; t.stateBrain = 5; t.YellForHelp = t.YellAtHero = t.RandomComment = 4;
    Reset(1000); t.Init();
    ok = std::strcmp(g_log, "follow1;scared1;marker HUD_ORB_QUEST_OPTIONAL;regionfollow120;change0;") == 0 && t.stateBrain == 1 && t.RandomComment == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00ea35a0_TEST FAIL\n"); return 1; }
    std::printf("00ea35a0_TEST PASS\n");
    return 0;
}