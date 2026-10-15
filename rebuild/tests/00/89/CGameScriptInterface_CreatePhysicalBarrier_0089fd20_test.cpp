// Behaviour test for 0x0089FD20 (CreatePhysicalBarrier): the reconstructed logic run over a host model of the thing, its component map and the callees.
#include <cstdio>
// Behaviour model of the script-interface plumbing: a thing with a component-presence bitset (+0x20 words in retail)
// and a sorted component map searched by lower bound (end entry when the kind is absent), a script thing with
// GetThing/IsValid, and the hero lookup chain PlayerManager -> MainPlayer -> hero creature.
struct TC { int kind; TC() : kind(0) {} virtual ~TC() {} };
struct Entry { long kind; TC* tc; };
struct Thing {
    unsigned int mask[8];
    Entry entries[9];                      // sorted; entries[count] is the end entry
    int count;
    unsigned char type;
    bool dead;
    Thing() : count(0), type(0), dead(false) { for (int i = 0; i < 8; ++i) mask[i] = 0; entries[0].kind = 0; entries[0].tc = 0; }
    void Add(long kind, TC* tc)
    {
        int i = count;
        while (i > 0 && entries[i - 1].kind > kind) { entries[i] = entries[i - 1]; --i; }
        entries[i].kind = kind; entries[i].tc = tc; ++count;
        entries[count].kind = 0; entries[count].tc = 0;
        mask[kind >> 5] |= 1u << (kind & 31);
        tc->kind = (int)kind;
    }
    unsigned int HasTC(long kind) const { return mask[kind >> 5] & (1u << (kind & 31)); }
    Entry* LowerBound(long kind) { int i = 0; while (i < count && entries[i].kind < kind) ++i; return &entries[i]; }
    TC* GetTC(long kind) { Entry* it = LowerBound(kind); Entry* last = &entries[count]; if (it == last || kind < it->kind) it = last; return it->tc; }
};
template <class T> bool GetTCOut(Thing* t, long kind, T** out) { if (t->HasTC(kind)) { *out = (T*)t->GetTC(kind); return true; } return false; }
struct ScriptThing {
    Thing* thing; bool valid;
    ScriptThing(Thing* t, bool v) : thing(t), valid(v) {}
    Thing* GetThing() const { return thing; }
    bool IsValid() const { return valid; }
};
inline bool IsThingAlive(const ScriptThing& s) { Thing* t = s.GetThing(); return t && !t->dead; }
struct Player { Thing* hero; Thing* GetHeroCreature() { return hero; } Thing* GetControlledCreature() { return hero; } };
struct PlayerManager { Player* main; Player* GetMainPlayer() { return main; } };
struct DefinitionManager { const char* known; long index; long GetDefGlobalIndexFromName(const char* n) const { const char* a = n; const char* b = known; while (*a && *a == *b) { ++a; ++b; } return (*a == *b) ? index : -1; } };
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Vec { float x, y, z; };
struct Made : Thing { const char* scriptName; const Vec* start; const Vec* end; float radius; };
static Made g_made;
static Thing* CreatePhysicalBarrierThing(const Vec& s, const Vec& e, float r) { g_made.start = &s; g_made.end = &e; g_made.radius = r; return &g_made; }
struct GSI {
    Thing* CreatePhysicalBarrier(float radius, const Vec& start, const Vec* end, const char* scriptName) const
    {
        Made* t = (Made*)CreatePhysicalBarrierThing(start, end ? *end : start, radius);
        if (t && !t->dead) { t->scriptName = scriptName; return t; }
        return 0;
    }
};
static void Run()
{
    GSI g; Vec a = { 0, 0, 0 }, b = { 5, 0, 0 };
    CHECK(g.CreatePhysicalBarrier(2.0f, a, &b, "WALL") == &g_made && g_made.start == &a && g_made.end == &b && g_made.radius == 2.0f && g_made.scriptName[0] == 'W');
    CHECK(g.CreatePhysicalBarrier(1.0f, a, 0, "POST") == &g_made && g_made.end == &a);
    g_made.dead = true; CHECK(g.CreatePhysicalBarrier(1.0f, a, 0, "POST") == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089fd20_TEST FAIL\n"); return 1; }
    std::printf("0089fd20_TEST PASS\n");
    return 0;
}