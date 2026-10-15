// Behaviour test for 0x00D7F110 (CHeroSoulsEnemy::Main): the call sequence of the script over a logging game interface.
#include <cstdio>
#include <cstring>
// Host model of the entity-script plumbing: the game interface logs every call, IsDone turns true after a
// configurable number of checks, the filter registration is counted.
struct ScriptThing { int id; };
struct Game {
    char log[64]; int n;
    const ScriptThing* hero; ScriptThing heroThing;
    Game() : n(0) { log[0] = 0; heroThing.id = 99; hero = &heroThing; }
    void Add(char c) { log[n++] = c; log[n] = 0; }
    void NewScriptFrame() { Add('F'); }
    void Pause(float s) { Add(s == 1.0f ? 'P' : (s == 0.5f ? 'p' : '?')); }
    const ScriptThing& GetHero() { Add('H'); return *hero; }
    const ScriptThing* lastTarget; const ScriptThing* lastThing;
    void GiveThingBestEnemyTarget(const ScriptThing& t, const ScriptThing& target) { Add('T'); lastThing = &t; lastTarget = &target; }
    void RemoveThing(const ScriptThing& t, bool a, bool b) { Add(a && b ? 'R' : (!a && b ? 'r' : '?')); lastThing = &t; }
};
struct ScriptBase {
    Game* game; ScriptThing Me; int filters; int doneAfter; int checks;
    ScriptBase(Game* g, int after) : game(g), filters(0), doneAfter(after), checks(0) { Me.id = 7; }
    void RegisterFilter() { ++filters; }
    bool IsDone() { return ++checks > doneAfter; }
};
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct HeroSoulsEnemy : ScriptBase {
    HeroSoulsEnemy(Game* g, int after) : ScriptBase(g, after) {}
    void Main()
    {
        RegisterFilter();
        game->NewScriptFrame();
        if (!IsDone()) {
            game->Pause(1.0f);
            game->GiveThingBestEnemyTarget(Me, game->GetHero());
            do { game->NewScriptFrame(); } while (!IsDone());
        }
    }
};
int main()
{
    Game g1; HeroSoulsEnemy a(&g1, 0); a.Main(); CHECK(std::strcmp(g1.log, "F") == 0 && a.filters == 1);
    Game g2; HeroSoulsEnemy b(&g2, 3); b.Main(); CHECK(std::strcmp(g2.log, "FPHTFFF") == 0 && g2.lastThing == &b.Me && g2.lastTarget == g2.hero);
    if (g_fail) { std::printf("00d7f110_TEST FAIL\n"); return 1; }
    std::printf("00d7f110_TEST PASS\n");
    return 0;
}
