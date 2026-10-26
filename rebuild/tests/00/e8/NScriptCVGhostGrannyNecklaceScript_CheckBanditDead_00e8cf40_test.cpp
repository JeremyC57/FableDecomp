// Behaviour test for 0x00E8CF40 (CV_GhostGrannyNecklaceScript::CheckBanditDead): wait for the kill, flag, objective.
#include <cstdio>
#include <cstring>
#include <string>
// Host model: logs 'F' frame, 'O' third objective (active quest, objective 03, OrchardFarm, ""); the bandit handle may
// be empty (never killed) or report the kill (by anyone: "" killer) from frame 'killedAt'.
struct Game;
struct Impl { Game* g; int killedAt; bool MsgIsKilledBy(const std::string& who) const; };
struct Game {
    std::string log; int frames; int releases;
    Game() : frames(0), releases(0) {}
    void NewScriptFrame() { log += "F"; ++frames; }
    std::string GetActiveQuestName() { return "Q_GhostGranny"; }
    void SetQuestCardObjective(const std::string& q, const std::string& t, const std::string& r1, const std::string& r2)
    { log += (q == "Q_GhostGranny" && t == "TEXT_QUEST_GHOST_GRANNY_NECKLACE_OBJECTIVE_03" && r1 == "OrchardFarm" && r2.empty()) ? "O" : "?"; }
};
bool Impl::MsgIsKilledBy(const std::string& who) const { return who.empty() && g->frames >= killedAt; }
struct Thing {
    Impl* Data; Game* g;
    Thing(Impl* d, Game* game) : Data(d), g(game) {}
    Thing(const Thing& o) : Data(o.Data), g(o.g) {}
    ~Thing() { ++g->releases; }
    bool MsgIsKilledBy(const std::string& k) const { if (!Data) return false; return Data->MsgIsKilledBy(k); }
};
struct Script {
    Game* GSI; bool BanditKilled; int termAfter; int checks;
    Script(Game* g, int after) : GSI(g), BanditKilled(false), termAfter(after), checks(0) {}
    bool IsActiveThreadTerminating() { return ++checks > termAfter; }
    void CheckBanditDead(Thing bandit)
    {
        while (!bandit.MsgIsKilledBy("")) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        BanditKilled = true;
        GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_GHOST_GRANNY_NECKLACE_OBJECTIVE_03", "OrchardFarm", "");
    }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    { Game g; Impl i = { &g, 2 }; Script s(&g, 9); { Thing t(&i, &g); s.CheckBanditDead(t); }
      CHECK(g.log == "FFO" && s.BanditKilled && g.releases == 2); }
    { Game g; Script s(&g, 3); { Thing t(0, &g); s.CheckBanditDead(t); } CHECK(g.log == "FFFF" && !s.BanditKilled); }
    { Game g; Impl i = { &g, 0 }; Script s(&g, 0); { Thing t(&i, &g); s.CheckBanditDead(t); } CHECK(g.log == "" && !s.BanditKilled); }
    if (g_fail) { std::printf("00e8cf40_TEST FAIL\n"); return 1; }
    std::printf("00e8cf40_TEST PASS\n");
    return 0;
}