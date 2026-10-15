// Behaviour test for 0x0089D600 (SetCutsceneMode): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Subtitles { int calls; bool in; void SetAsInCutscene(bool b) { ++calls; in = b; } };
struct PlayerGui { Subtitles subs; bool inCutscene; };
static PlayerGui g_gui;
struct Reset : TC { int calls; Reset() : calls(0) {} void DoReset() { ++calls; } };
struct GSI {
    PlayerManager* PM; int envCalls; bool envArg;
    void SetEnvironmentalEffectsAlwaysUpdate(bool b) { ++envCalls; envArg = b; }
    void SetCutsceneMode(bool inCutscene, bool flag)
    {
        g_gui.subs.SetAsInCutscene(inCutscene);
        g_gui.inCutscene = flag;
        Thing* hero = PM->GetMainPlayer()->GetHeroCreature();
        if (hero && !hero->dead && hero->HasTC(0x30)) ((Reset*)hero->GetTC(0x30))->DoReset();
        SetEnvironmentalEffectsAlwaysUpdate(inCutscene);
    }
};
static void Run()
{
    g_gui.subs.calls = 0; Thing hero; Reset r; hero.Add(0x30, &r); Player p; p.hero = &hero; PlayerManager pm; pm.main = &p;
    GSI g; g.PM = &pm; g.envCalls = 0;
    g.SetCutsceneMode(true, false); CHECK(g_gui.subs.calls == 1 && g_gui.subs.in && !g_gui.inCutscene && r.calls == 1 && g.envCalls == 1 && g.envArg);
    hero.dead = true; g.SetCutsceneMode(false, true); CHECK(g_gui.subs.calls == 2 && !g_gui.subs.in && g_gui.inCutscene && r.calls == 1 && g.envCalls == 2 && !g.envArg);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089d600_TEST FAIL\n"); return 1; }
    std::printf("0089d600_TEST PASS\n");
    return 0;
}
