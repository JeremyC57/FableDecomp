#include <cstdio>
#include <cstring>
// Model of CTCCreatureGenerator::InitialiseCreatureGenerator: unless restoring, the exclusion area is reset and taken from the
// first live BAD_CREATURE_GENERATION_AREA marker that has a shape manager; the open (hidden=false) then hidden (hidden=true)
// positions are gathered with the current exclusion area; a live hero with the global nav-check switch and the pending flag runs
// DoNavCheck once and clears the flag; a trigger-on-activate generator creates its creatures; Initialised is set last.
struct Shape { int id; };
struct Marker { bool dead; Shape* shape; };
static Marker* g_markers[4]; static int g_nmarkers = 0;
static const char* g_lookup = 0;
static int g_calls[8]; static int g_ncalls = 0;           // 1 = open, 2 = hidden, 3 = nav, 4 = create
static const Shape* g_seen[2];
struct Hero { bool dead; };
static Hero* g_hero = 0;
static bool g_navEnabled = true;
struct Gen {
    Shape* PExclusionArea; bool NeedToDoNavCheck, TriggerOnActivate, Initialised;
    void GetValid(bool hidden, const Shape* exclusion) { g_seen[hidden ? 1 : 0] = exclusion; g_calls[g_ncalls++] = hidden ? 2 : 1; }
    void Init(bool restoring)
    {
        if (!restoring) {
            PExclusionArea = 0;
            g_lookup = "MARKER_SET_BAD_CREATURE_GENERATION_AREA";
            if (g_nmarkers > 0) {
                Marker* m = g_markers[0];
                if (m && !m->dead && m->shape) PExclusionArea = m->shape;
            }
        }
        GetValid(false, PExclusionArea);
        GetValid(true, PExclusionArea);
        if (g_hero && !g_hero->dead && g_navEnabled && NeedToDoNavCheck) { g_calls[g_ncalls++] = 3; NeedToDoNavCheck = false; }
        if (TriggerOnActivate) g_calls[g_ncalls++] = 4;
        Initialised = true;
    }
};
int main()
{
    int bad = 0;
    Shape s = { 7 }; Marker m = { false, &s }; g_markers[0] = &m; g_nmarkers = 1;
    Hero h = { false }; g_hero = &h;
    Gen g; g.PExclusionArea = (Shape*)1; g.NeedToDoNavCheck = true; g.TriggerOnActivate = true; g.Initialised = false;
    g.Init(false);
    if (g.PExclusionArea != &s || g_seen[0] != &s || g_seen[1] != &s) bad = 1;
    if (g_ncalls != 4 || g_calls[0] != 1 || g_calls[1] != 2 || g_calls[2] != 3 || g_calls[3] != 4) bad = 1;
    if (g.NeedToDoNavCheck || !g.Initialised || !g_lookup || std::strcmp(g_lookup, "MARKER_SET_BAD_CREATURE_GENERATION_AREA") != 0) bad = 1;
    // restoring: the exclusion area is kept, no nav check once cleared, no trigger
    g_ncalls = 0; g.TriggerOnActivate = false; Shape t = { 9 }; g.PExclusionArea = &t;
    g.Init(true);
    if (g.PExclusionArea != &t || g_seen[1] != &t || g_ncalls != 2) bad = 1;
    // a dead marker leaves the area empty; a dead hero skips the nav check
    g_ncalls = 0; m.dead = true; h.dead = true; g.NeedToDoNavCheck = true;
    g.Init(false);
    if (g.PExclusionArea != 0 || g_seen[0] != 0 || g_ncalls != 2 || !g.NeedToDoNavCheck) bad = 1;
    if (bad) { std::printf("0074b770_TEST FAIL\n"); return 1; }
    std::printf("0074b770_TEST PASS\n");
    return 0;
}
