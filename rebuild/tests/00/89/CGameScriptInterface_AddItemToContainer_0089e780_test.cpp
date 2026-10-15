// Behaviour test for 0x0089E780 (AddItemToContainer): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Container : TC { long def; int calls; char extra0; Container() : def(0), calls(0), extra0('x') {} void InsertItem(long d, const char* e) { def = d; extra0 = e[0]; ++calls; } };
struct GSI {
    DefinitionManager* DefMgr;
    void AddItemToContainer(const ScriptThing& thing, const char* name) const
    {
        if (thing.IsValid()) {
            long defIndex = DefMgr->GetDefGlobalIndexFromName(name);
            if (defIndex > 0) {
                Thing* t = thing.GetThing();
                if (t->HasTC(0x3c)) { Container* c = (Container*)t->GetTC(0x3c); c->InsertItem(defIndex, ""); }
            }
        }
    }
};
static void Run()
{
    Thing chest; Container c; chest.Add(0x3c, &c); ScriptThing s(&chest, true); DefinitionManager dm; dm.known = "OBJECT_KEY"; dm.index = 8; GSI g; g.DefMgr = &dm;
    g.AddItemToContainer(s, "OBJECT_KEY"); CHECK(c.calls == 1 && c.def == 8 && c.extra0 == 0);
    g.AddItemToContainer(s, "NOPE"); CHECK(c.calls == 1);
    s.valid = false; g.AddItemToContainer(s, "OBJECT_KEY"); CHECK(c.calls == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089e780_TEST FAIL\n"); return 1; }
    std::printf("0089e780_TEST PASS\n");
    return 0;
}