// Behaviour test for 0x0089EAA0 (CreatureGeneratorSetGeneratedCreatureScriptName): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Generator : TC { bool triggered; bool always; int alwaysCalls; const char* name; long numTriggers; bool destroyed;
    Generator() : triggered(false), always(false), alwaysCalls(0), name(0), numTriggers(0), destroyed(false) {}
    void SetAlwaysCreateCreaturesOnTrigger(bool a) { always = a; ++alwaysCalls; } };
struct GSI {
    void Do(const ScriptThing& s, const char* n) const { Generator* gen; if (s.IsValid() && IsThingAlive(s) && GetTCOut(s.GetThing(), 0x3d, &gen)) gen->name = n; }
};
static void Run()
{
    Thing c; Generator gen; c.Add(0x3d, &gen); ScriptThing s(&c, true); GSI g;
    g.Do(s, "BANDIT"); CHECK(gen.name != 0 && gen.name[0] == 'B'); ScriptThing none(0, true); g.Do(none, "X"); CHECK(gen.name[0] == 'B');
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089eaa0_TEST FAIL\n"); return 1; }
    std::printf("0089eaa0_TEST PASS\n");
    return 0;
}
