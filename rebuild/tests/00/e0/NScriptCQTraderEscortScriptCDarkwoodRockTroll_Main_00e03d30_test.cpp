// Behaviour test for CQ_TraderEscortScript::CDarkwoodRockTroll::Main 0x00E03D30 (model copy with a fake game interface).
#include <cstdio>
#include <cstring>
struct Thing { int id; };
struct Game {
    Thing hero; float health[8]; int frame; int predicateSet; const Thing* target; const Thing* attacker;
    char objective[4][64];
    void NewScriptFrame() { ++frame; }
    Thing& GetHero() { return hero; }
    float GetHealth(const Thing&) { return health[frame < 7 ? frame : 7]; }
    void GiveThingBestEnemyTarget(const Thing& t, const Thing& enemy) { attacker = &t; target = &enemy; }
    const char* GetActiveQuestName() { return "Q_TraderEscort"; }
    void SetQuestCardObjective(const char* q, const char* a, const char* b, const char* c)
    { std::strcpy(objective[0], q); std::strcpy(objective[1], a); std::strcpy(objective[2], b); std::strcpy(objective[3], c); }
};
struct Quest { bool ShownEarthTroll; };
struct Troll {
    Game* G; Thing Me; Quest* ParentClass; int terminateAt; int checks;
    bool IsActiveThreadTerminating() { ++checks; return terminateAt >= 0 && checks >= terminateAt; }
    void Main()
    {
        G->predicateSet = 1;
        G->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        G->GiveThingBestEnemyTarget(Me, G->GetHero());
        while (!(G->GetHealth(Me) <= 0.0f)) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        G->SetQuestCardObjective(G->GetActiveQuestName(), "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03", "BarrowFields", "BarrowFields");
        ParentClass->ShownEarthTroll = false;
    }
};
int main()
{
    Game g; std::memset(&g, 0, sizeof g);
    float hp[8] = { 100, 100, 80, 40, 10, 0, 0, 0 };
    std::memcpy(g.health, hp, sizeof hp);
    Quest q; q.ShownEarthTroll = true;
    Troll t; t.G = &g; t.Me.id = 5; t.ParentClass = &q; t.terminateAt = -1; t.checks = 0;
    t.Main();
    bool ok = !q.ShownEarthTroll && g.frame == 5 && g.target == &g.hero && g.attacker == &t.Me
        && std::strcmp(g.objective[1], "TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_03") == 0 && std::strcmp(g.objective[3], "BarrowFields") == 0;
    Game h; std::memset(&h, 0, sizeof h); std::memcpy(h.health, hp, sizeof hp);
    Quest r; r.ShownEarthTroll = true;
    Troll u; u.G = &h; u.Me.id = 6; u.ParentClass = &r; u.terminateAt = 2; u.checks = 0;
    u.Main();
    if (!r.ShownEarthTroll || h.objective[0][0]) ok = false;
    if (!ok) { std::printf("00e03d30_TEST FAIL\n"); return 1; }
    std::printf("00e03d30_TEST PASS\n");
    return 0;
}