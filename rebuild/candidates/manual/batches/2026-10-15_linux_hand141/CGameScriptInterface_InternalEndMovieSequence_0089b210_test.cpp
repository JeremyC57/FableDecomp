// Behaviour test for 0x0089B210 (InternalEndMovieSequence): the reconstructed logic run over a host model of the thing, its component map and the callees.
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
struct Boast : TC { int calls; bool allow; Boast() : calls(0), allow(false) {} void AllowUIToOpen(bool a) { ++calls; allow = a; } };
struct Camera { int ended; Camera() : ended(0) {} void OnMovieSequenceEnded() { ++ended; } };
struct GSI {
    PlayerManager* PM; Camera* cam; bool InMovieSequence; char log[8]; int n;
    void EndLetterBox() { log[n++] = 'L'; }
    void ResumeAbilitiesForCutscene() { log[n++] = 'R'; }
    void CameraResetToViewBehindHero(bool b) { log[n++] = b ? 'b' : 'B'; }
    void CameraDefault() { log[n++] = 'D'; }
    void InternalEndMovieSequence()
    {
        InMovieSequence = false;
        EndLetterBox();
        cam->OnMovieSequenceEnded();
        ResumeAbilitiesForCutscene();
        CameraResetToViewBehindHero(false);
        CameraDefault();
        Thing* hero = PM->GetMainPlayer()->GetHeroCreature();
        Boast* boast;
        if (hero && !hero->dead && GetTCOut(hero, 0xe8, &boast)) boast->AllowUIToOpen(true);
    }
};
static void Run()
{
    Thing hero; Boast b; hero.Add(0xe8, &b); Camera c;
    Player p; p.hero = &hero; PlayerManager pm; pm.main = &p;
    GSI g; g.PM = &pm; g.cam = &c; g.InMovieSequence = true; g.n = 0;
    g.InternalEndMovieSequence();
    CHECK(!g.InMovieSequence && c.ended == 1 && g.n == 4 && g.log[0] == 'L' && g.log[1] == 'R' && g.log[2] == 'B' && g.log[3] == 'D');
    CHECK(b.calls == 1 && b.allow);
    g.n = 0; hero.dead = true; g.InternalEndMovieSequence(); CHECK(b.calls == 1 && c.ended == 2);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0089b210_TEST FAIL\n"); return 1; }
    std::printf("0089b210_TEST PASS\n");
    return 0;
}
