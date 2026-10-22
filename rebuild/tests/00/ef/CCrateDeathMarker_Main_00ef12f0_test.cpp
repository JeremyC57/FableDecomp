// Behaviour test for 0x00EF12F0 CCrateDeathMarker::Main: runs the reconstructed body against a stub game interface and
// checks the alive filter registration over Me, the opening script frame and early IsDone return, the 5.5 approach wait
// (one frame per poll, hero must be alive), the CREATURE_BS_VILLAGER_MALE spawn at Me's position with an empty script
// name, the fade-in (alpha 0, then 0 .. 1 in 0.05 steps, one frame each), the 8.0 departure wait, the round-closing frame,
// and that the villager is released on every exit (fade top/bottom, post-fade, wait loop, post-wait, scope end).
#include <cstdio>
#include <cstring>

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)

struct C3DVector { float X, Y, Z; };

static int g_liveVillagers = 0;
static int g_created = 0;

struct CScriptThing {
    int  Id;          // 0 = Me / hero, 1 = villager
    bool Owned;
    CScriptThing() : Id(0), Owned(false) { }
    explicit CScriptThing(int id) : Id(id), Owned(id == 1) { if (Owned) ++g_liveVillagers; }
    CScriptThing(const CScriptThing& o) : Id(o.Id), Owned(o.Owned) { if (Owned) ++g_liveVillagers; }
    ~CScriptThing() { if (Owned) --g_liveVillagers; }
    const C3DVector& GetPos() const { static C3DVector p = { 1.0f, 2.0f, 3.0f }; return p; }
    bool IsAlive() const;
};

static int  g_frame = 0;
static int  g_doneAt = 1000;
static int  g_nearFrom = 0, g_nearTo = 1000;   // within 5.5 while g_nearFrom <= frame < g_nearTo
static int  g_farFrom = 0;                    // beyond 8 when frame >= g_farFrom
static int  g_heroDeadFrom = 1000;
static int  g_alphaCalls = 0;
static float g_alphas[256];
static int  g_filters = 0;
static bool g_badSpawn = false;

bool CScriptThing::IsAlive() const { return g_frame < g_heroDeadFrom; }

static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d)
{
    if (d != 5.5f) g_badSpawn = true;
    return g_frame >= g_nearFrom && g_frame < g_nearTo;
}
static bool IsDistanceBetweenThingsOver(const CScriptThing&, const CScriptThing&, float d)
{
    if (d != 8.0f) g_badSpawn = true;
    return g_frame >= g_farFrom;
}

struct CGameScriptInterface {
    CScriptThing Hero;
    void NewScriptFrame() const { ++g_frame; }
    const CScriptThing& GetHero() const { return Hero; }
    CScriptThing CreateCreature(const char* def, const C3DVector& pos, const char* scriptName, bool flag) const
    {
        if (std::strcmp(def, "CREATURE_BS_VILLAGER_MALE") != 0 || std::strcmp(scriptName, "") != 0 || flag ||
            pos.X != 1.0f || pos.Y != 2.0f || pos.Z != 3.0f)
            g_badSpawn = true;
        ++g_created;
        return CScriptThing(1);
    }
    void EntitySetAlpha(const CScriptThing& thing, float alpha, bool fade) const
    {
        if (thing.Id != 1 || !fade) g_badSpawn = true;
        if (g_alphaCalls < 256) g_alphas[g_alphaCalls] = alpha;
        ++g_alphaCalls;
    }
};

struct CIsThingAlive {
    CScriptThing m_thing;
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
};

struct CCrateDeathMarker {
    CGameScriptInterface* Game;
    CScriptThing          Me;
    void RegisterFilter(CIsThingAlive& filter) { if (filter.m_thing.Id == 0) ++g_filters; }
    bool IsDone() { return g_frame >= g_doneAt; }
    void Main();
};

void CCrateDeathMarker::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDone()) {
        while (!(Game->GetHero().IsAlive() && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.5f))) {
            Game->NewScriptFrame();
            if (IsDone())
                return;
        }
        if (IsDone())
            return;
        {
            CScriptThing villager = Game->CreateCreature("CREATURE_BS_VILLAGER_MALE", Me.GetPos(), "", false);
            Game->EntitySetAlpha(villager, 0.0f, true);
            for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f) {
                if (IsDone())
                    return;
                Game->EntitySetAlpha(villager, alpha, true);
                Game->NewScriptFrame();
                if (IsDone())
                    return;
            }
            if (IsDone())
                return;
            while (Game->GetHero().IsAlive() && !IsDistanceBetweenThingsOver(Me, Game->GetHero(), 8.0f)) {
                Game->NewScriptFrame();
                if (IsDone())
                    return;
            }
            if (IsDone())
                return;
        }
        Game->NewScriptFrame();
    }
}

static int FadeSteps()
{
    int n = 0;
    for (float alpha = 0.0f; alpha <= 1.0f; alpha += 0.05f)
        ++n;
    return n;
}

static void Run(int doneAt, int nearFrom, int nearTo, int farFrom, int heroDeadFrom)
{
    g_frame = 0; g_doneAt = doneAt; g_nearFrom = nearFrom; g_nearTo = nearTo; g_farFrom = farFrom;
    g_heroDeadFrom = heroDeadFrom; g_alphaCalls = 0; g_filters = 0; g_created = 0; g_liveVillagers = 0; g_badSpawn = false;
    CGameScriptInterface game;
    CCrateDeathMarker crate;
    crate.Game = &game;
    crate.Main();
}

int main()
{
    int steps = FadeSteps();
    CHECK(steps >= 20 && steps <= 21);

    // done right after the opening frame: filter only
    Run(1, 0, 1000, 0, 1000);
    CHECK(g_filters == 1); CHECK(g_frame == 1); CHECK(g_created == 0); CHECK(g_liveVillagers == 0);

    // hero arrives at frame 5, done in the middle of the fade (fade-bottom exit)
    Run(8, 5, 1000, 1000, 1000);
    CHECK(g_created == 1); CHECK(g_liveVillagers == 0); CHECK(g_frame == 8);
    CHECK(g_alphaCalls == 4);
    CHECK(g_alphas[0] == 0.0f); CHECK(g_alphas[1] == 0.0f); CHECK(g_alphas[2] == 0.05f);
    CHECK(!g_badSpawn);

    // two full rounds, then done while waiting for the hero (approach-wait exit, no villager alive)
    Run(60, 5, 40, 30, 1000);
    CHECK(g_created == 2); CHECK(g_liveVillagers == 0); CHECK(g_frame == 60);
    CHECK(g_alphaCalls == 2 * (1 + steps));
    CHECK(!g_badSpawn);

    // done while waiting for the hero to leave (departure-wait exit)
    Run(5 + steps + 3, 5, 1000, 1000, 1000);
    CHECK(g_created == 1); CHECK(g_liveVillagers == 0); CHECK(g_frame == 5 + steps + 3);

    // dead hero: no spawn until he is alive; dead after the fade ends the departure wait at once
    Run(5 + steps + 1, 0, 1000, 1000, 5 + steps);
    CHECK(g_created == 1); CHECK(g_liveVillagers == 0); CHECK(g_frame == 5 + steps + 1);
    Run(10, 0, 1000, 1000, 0);
    CHECK(g_created == 0); CHECK(g_frame == 10);

    if (g_failures) {
        std::printf("00ef12f0_TEST FAIL\n");
        return 1;
    }
    std::printf("00ef12f0_TEST PASS\n");
    return 0;
}