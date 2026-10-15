// Behaviour test for 0x0089D3A0 (EntityAttachToVillage): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Village : TC { Thing* added; void AddThingToVillage(Thing* t) { added = t; } };
struct GSI {
    void EntityAttachToVillage(const ScriptThing& thing, const ScriptThing& village) const
    {
        if (thing.IsValid() && village.IsValid() && thing.GetThing()->HasTC(0x23)) {
            Thing* v = village.GetThing();
            if (v->HasTC(0x22)) ((Village*)v->GetTC(0x22))->AddThingToVillage(thing.GetThing());
        }
    }
};
static void Run()
{
    Thing person, town; TC villager; person.Add(0x23, &villager); Village vc; vc.added = 0; town.Add(0x22, &vc);
    ScriptThing sp(&person, true), st(&town, true); GSI g;
    g.EntityAttachToVillage(sp, st); CHECK(vc.added == &person);
    vc.added = 0; Thing rock; ScriptThing sr(&rock, true); g.EntityAttachToVillage(sr, st); CHECK(vc.added == 0);
    st.valid = false; g.EntityAttachToVillage(sp, st); CHECK(vc.added == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089d3a0_TEST FAIL\n"); return 1; }
    std::printf("0089d3a0_TEST PASS\n");
    return 0;
}