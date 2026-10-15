// Behaviour test for 0x0089ED80 (IsCreatureFlying): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Modes : TC { long modes[2]; int n; bool HasMode(long m) { for (int i = 0; i < n; ++i) if (modes[i] == m) return true; return false; } };
struct GSI {
    bool IsCreatureFlying(const ScriptThing& creature) const
    {
        if (creature.IsValid()) {
            Thing* t = creature.GetThing();
            if (t->HasTC(0x31)) {
                Modes* m = (Modes*)t->GetTC(0x31);
                if (m->HasMode(0x50) || m->HasMode(0x4d)) return true;
            }
        }
        return false;
    }
};
static void Run()
{
    Thing bird; Modes m; m.n = 0; bird.Add(0x31, &m); ScriptThing s(&bird, true); GSI g;
    CHECK(!g.IsCreatureFlying(s));
    m.modes[0] = 0x4d; m.n = 1; CHECK(g.IsCreatureFlying(s));
    m.modes[0] = 0x50; CHECK(g.IsCreatureFlying(s));
    m.modes[0] = 0x22; CHECK(!g.IsCreatureFlying(s));
    s.valid = false; m.modes[0] = 0x50; CHECK(!g.IsCreatureFlying(s));
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089ed80_TEST FAIL\n"); return 1; }
    std::printf("0089ed80_TEST PASS\n");
    return 0;
}
