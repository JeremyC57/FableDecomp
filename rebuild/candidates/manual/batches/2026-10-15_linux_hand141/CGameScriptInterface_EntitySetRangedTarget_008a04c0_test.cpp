// Behaviour test for 0x008A04C0 (EntitySetRangedTarget): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Targeting : TC { Thing* target; int calls; Targeting() : target(0), calls(0) {} void SetRangedTarget(Thing* t) { target = t; ++calls; } };
struct GSI {
    void EntitySetRangedTarget(const ScriptThing& thing, const ScriptThing& target) const
    {
        if (thing.IsValid()) {
            Thing* t = thing.GetThing();
            if (t->HasTC(0x8)) {
                Targeting* tg = (Targeting*)t->GetTC(0x8);
                if (target.IsValid()) tg->SetRangedTarget(target.GetThing()); else tg->SetRangedTarget(0);
            }
        }
    }
};
static void Run()
{
    Thing archer, deer; Targeting tg; archer.Add(0x8, &tg); ScriptThing sa(&archer, true), sd(&deer, true); GSI g;
    g.EntitySetRangedTarget(sa, sd); CHECK(tg.calls == 1 && tg.target == &deer);
    sd.valid = false; g.EntitySetRangedTarget(sa, sd); CHECK(tg.calls == 2 && tg.target == 0);
    sa.valid = false; g.EntitySetRangedTarget(sa, sd); CHECK(tg.calls == 2);
}

int main()
{
    Run();
    if (g_fail) { std::printf("008a04c0_TEST FAIL\n"); return 1; }
    std::printf("008a04c0_TEST PASS\n");
    return 0;
}
