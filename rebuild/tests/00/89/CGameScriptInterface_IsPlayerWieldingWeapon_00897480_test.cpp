// Behaviour test for 0x00897480 (IsPlayerWieldingWeapon): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Carrying : TC { Thing* held; Thing* GetThingInPrimarySlot() { return held; } };
struct GSI {
    PlayerManager* PM;
    bool IsPlayerWieldingWeapon() const
    {
        Thing* hero = PM->GetMainPlayer()->GetHeroCreature();
        if (hero && !hero->dead && hero->HasTC(0x46)) {
            Thing* weapon = ((Carrying*)hero->GetTC(0x46))->GetThingInPrimarySlot();
            return weapon && weapon->HasTC(0x48);
        }
        return false;
    }
};
static void Run()
{
    Thing hero, sword, stick; TC weaponTC; sword.Add(0x48, &weaponTC);
    Carrying c; c.held = &sword; hero.Add(0x46, &c);
    Player p; p.hero = &hero; PlayerManager pm; pm.main = &p; GSI g; g.PM = &pm;
    CHECK(g.IsPlayerWieldingWeapon());
    c.held = &stick; CHECK(!g.IsPlayerWieldingWeapon());
    c.held = 0; CHECK(!g.IsPlayerWieldingWeapon());
    c.held = &sword; hero.dead = true; CHECK(!g.IsPlayerWieldingWeapon());
    Thing bare; p.hero = &bare; CHECK(!g.IsPlayerWieldingWeapon());
}

int main()
{
    Run();
    if (g_fail) { std::printf("00897480_TEST FAIL\n"); return 1; }
    std::printf("00897480_TEST PASS\n");
    return 0;
}