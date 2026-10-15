// Behaviour test for 0x00E95E80 (CHHUndead::Main): the call sequence of the script over a logging game interface.
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
struct HauntedHouse { bool raise; };
struct HHUndead : ScriptBase {
    HauntedHouse* parent;
    HHUndead(Game* g, int after, HauntedHouse* p) : ScriptBase(g, after), parent(p) {}
    void Main()
    {
        RegisterFilter();
        game->NewScriptFrame();
        if (IsDone()) return;
        if (parent->raise) {
            if (IsDone()) return;
            game->RemoveThing(Me, true, true);
        }
        do { game->NewScriptFrame(); } while (!IsDone());
    }
};
int main()
{
    HauntedHouse off = { false }, on = { true };
    Game g1; HHUndead a(&g1, 0, &on); a.Main(); CHECK(std::strcmp(g1.log, "F") == 0 && a.filters == 1);
    Game g2; HHUndead b(&g2, 2, &off); b.Main(); CHECK(std::strcmp(g2.log, "FFF") == 0);
    Game g3; HHUndead c(&g3, 3, &on); c.Main(); CHECK(std::strcmp(g3.log, "FRFF") == 0 && g3.lastThing == &c.Me);
    Game g4; HHUndead d(&g4, 1, &on); d.Main(); CHECK(std::strcmp(g4.log, "F") == 0);
    if (g_fail) { std::printf("00e95e80_TEST FAIL\n"); return 1; }
    std::printf("00e95e80_TEST PASS\n");
    return 0;
}