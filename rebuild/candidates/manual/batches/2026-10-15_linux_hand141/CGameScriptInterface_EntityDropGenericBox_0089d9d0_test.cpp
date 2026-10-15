// Behaviour test for 0x0089D9D0 (EntityDropGenericBox): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Carrying : TC { int removed; bool arg; Carrying() : removed(0), arg(true) {} Thing* RemoveThingFromPrimarySlot(bool b) { ++removed; arg = b; return 0; } };
struct Modes : TC { unsigned long removedMode; Modes() : removedMode(0) {} void RemoveMode(unsigned long m) { removedMode = m; } };
struct GSI {
    void EntityDropGenericBox(const ScriptThing& thing) const
    {
        if (thing.IsValid()) {
            Thing* t = thing.GetThing();
            Carrying* carrying;
            if (GetTCOut(t, 0x46, &carrying)) {
                carrying->RemoveThingFromPrimarySlot(false);
                Modes* modes;
                if (GetTCOut(t, 0x31, &modes)) modes->RemoveMode(0x22);
            }
        }
    }
};
static void Run()
{
    Thing c; Carrying car; Modes m; c.Add(0x46, &car); c.Add(0x31, &m); ScriptThing s(&c, true); GSI g;
    g.EntityDropGenericBox(s); CHECK(car.removed == 1 && !car.arg && m.removedMode == 0x22);
    Thing c2; Modes m2; c2.Add(0x31, &m2); ScriptThing s2(&c2, true); g.EntityDropGenericBox(s2); CHECK(m2.removedMode == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089d9d0_TEST FAIL\n"); return 1; }
    std::printf("0089d9d0_TEST PASS\n");
    return 0;
}
