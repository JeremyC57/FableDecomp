// Behaviour test for 0x0089C710 (EntityPostOpinionDeedToRecipientVillage): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct PosThing : Thing { Vec pos; const Vec& GetPosition() const { return pos; } };
struct DeedLog : TC {
    long deed; const Vec* pos; Thing* target; Thing* victim; long postType; bool flagged; int calls;
    DeedLog() : calls(0) {}
    long PostDeed(long d, const Vec& p, Thing* t, Thing* v, long pt, bool f, bool, void*, bool, long) { deed = d; pos = &p; target = t; victim = v; postType = pt; flagged = f; ++calls; return 77; }
};
struct GSI {
    void Post(const ScriptThing& thing, long deedType, const ScriptThing& recipient, long postType) const
    {
        PosThing* t = (PosThing*)thing.GetThing();
        Thing* r = recipient.GetThing();
        if (t && r && r->type == 1 && r->HasTC(0x4f) && t->HasTC(0x51)) {
            DeedLog* log = (DeedLog*)t->GetTC(0x51);
            bool flagged = deedType >= 3 && deedType <= 10;
            log->PostDeed(deedType, t->GetPosition(), r, r, postType, flagged, false, 0, false, 0);
        }
    }
};
static void Run()
{
    PosThing hero; DeedLog log; hero.Add(0x51, &log); Thing villager; villager.type = 1; TC op; villager.Add(0x4f, &op);
    ScriptThing sh(&hero, true), sv(&villager, true); GSI g;
    g.Post(sh, 5, sv, 4); CHECK(log.calls == 1 && log.deed == 5 && log.flagged && log.target == &villager && log.victim == &villager && log.postType == 4 && log.pos == &hero.pos);
    g.Post(sh, 11, sv, 1); CHECK(log.calls == 2 && !log.flagged);
    g.Post(sh, 2, sv, 1); CHECK(!log.flagged);
    villager.type = 2; g.Post(sh, 5, sv, 1); CHECK(log.calls == 3);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089c710_TEST FAIL\n"); return 1; }
    std::printf("0089c710_TEST PASS\n");
    return 0;
}
