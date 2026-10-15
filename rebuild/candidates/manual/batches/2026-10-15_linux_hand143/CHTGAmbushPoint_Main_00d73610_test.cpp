// Behaviour test for 0x00D73610 (CHTG_AmbushPoint::Main): wait for the tracked thing to come near, then spawn, pause and remove.
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
struct Thing { float x; const char* data; };
static int g_distanceChecks = 0, g_nearAfter = 0;
static bool IsDistanceBetweenThingsUnder(const Thing&, const Thing&, float d) { return d == 15.0f && ++g_distanceChecks > g_nearAfter; }
struct Quest { Thing tracked; int spawned; const char* name; float pos; Quest() : spawned(0), name(0), pos(0) {} void SpawnEnemies(float p, const char* n, int count) { spawned = count; name = n; pos = p; } };
struct AmbushPoint : ScriptBase {
    Quest* parent; Thing me;
    AmbushPoint(Game* g, int after, Quest* q) : ScriptBase(g, after), parent(q) { me.x = 3.5f; me.data = "BANDITS"; }
    void Main()
    {
        while (!IsDistanceBetweenThingsUnder(me, parent->tracked, 15.0f)) {
            game->NewScriptFrame();
            if (IsDone()) return;
        }
        if (IsDone()) return;
        parent->SpawnEnemies(me.x, me.data, 4);
        game->Pause(0.5f);
        game->RemoveThing(Me, false, true);
    }
};
int main()
{
    Game g1; Quest q1; g_distanceChecks = 0; g_nearAfter = 2; AmbushPoint a(&g1, 10, &q1); a.Main();
    CHECK(std::strcmp(g1.log, "FFpr") == 0 && q1.spawned == 4 && std::strcmp(q1.name, "BANDITS") == 0 && q1.pos == 3.5f);
    Game g2; Quest q2; g_distanceChecks = 0; g_nearAfter = 5; AmbushPoint b(&g2, 1, &q2); b.Main();
    CHECK(std::strcmp(g2.log, "FF") == 0 && q2.spawned == 0);
    if (g_fail) { std::printf("00d73610_TEST FAIL\n"); return 1; }
    std::printf("00d73610_TEST PASS\n");
    return 0;
}
