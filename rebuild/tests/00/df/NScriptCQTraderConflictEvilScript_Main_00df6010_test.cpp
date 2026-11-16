// Behaviour test for NScript::CQ_TraderConflictEvilScript::Main (0x00DF6010): wait for Barrow Fields, slum beds unusable, the
// intro cut-scene from the hero's side (SOUTH / NORTH), the quest start screen once, every live enemy into the monster faction with
// the shopkeepers replaced by fake ones (and, per fake, the first shopkeeper found by the AllCreatures[j] search erased: the
// original script's index bug is kept), then until 25 guards are down: a creature hit by the hero (or by a non-heal special ability)
// or near the bandit follower sends the guards near it after the hero; one hit by a bandit fighter sends them after the nearest
// fighter; finally MissionSucceeded, the outro, beds usable again, quest completed and V_SickChildBarrowFields activated when
// V_SickChild is active. A state model of the script's decisions (the row's own code needs the game); terminate checks return.
#include <cstdio>
#include <cstring>
#include <vector>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Creature { int id; bool guard, shopkeeper, hit_hero, special_hero, heal, near_follower, hit_fighter; int x; };
struct Script {
    int Frame, LoadedAt, TerminateAt, Beds, Spawned, Initial, Killed, KillPerFrame, DownPerFrame;
    bool South, QuestStartScreened, MissionSucceeded, SickChildActive;
    std::vector<Creature> All;
    Script() : Frame(0), LoadedAt(0), TerminateAt(0), Beds(0), Spawned(0), Initial(0), Killed(0), KillPerFrame(0), DownPerFrame(0),
        South(false), QuestStartScreened(false), MissionSucceeded(false), SickChildActive(false) { }
    bool T() const { return Frame >= TerminateAt; }
    void NewScriptFrame() { ++Frame; }
    long Remaining() const { return (long)All.size() - Initial - Spawned + 25; }
    static bool Near(const Creature& a, const Creature& b) { int d = a.x - b.x; return d < 10 && d > -10; }
    void UpdateLiveEnemies() { for (int i = 0; i < Killed && !All.empty(); ++i) All.pop_back(); Killed = 0; }
    void Main()
    {
        while (Frame < LoadedAt) { NewScriptFrame(); if (T()) return; }
        if (T()) return;
        for (long b = 0; b < Beds; ++b) { if (T()) return; Log("bed_off"); }
        if (T()) return;
        UpdateLiveEnemies();
        if (South) { if (T()) return; Log("CS_SOUTH"); } else { if (T()) return; Log("CS_NORTH"); }
        if (!QuestStartScreened) { if (T()) return; Log("start_screen"); QuestStartScreened = true; }
        long fakes = 0;
        for (unsigned long i = 0; i < All.size(); ++i) {
            if (T()) return;
            Log("faction");
            if (!All[i].guard) {
                if (T()) return;
                if (All[i].shopkeeper) { if (T()) return; ++fakes; Log("fake"); }
                Log("stategroups_off");
            }
        }
        if (T()) return;
        for (long j = 0; j < fakes; ++j) {
            if (T()) return;
            for (unsigned long k = 0; k < All.size(); ++k) {
                if (T()) return;
                if (All[j].shopkeeper) {
                    if (T()) return;
                    for (std::vector<Creature>::iterator it = All.begin(); it != All.end(); ++it)
                        if (it->id == All[j].id) { All.erase(it); Log("erase"); break; }
                    break;
                }
            }
            if (T()) return;
        }
        if (T()) return;
        UpdateLiveEnemies();
        Initial = (int)All.size();
        while (Remaining() > 0) {
            NewScriptFrame();
            if (T()) return;
            Killed = KillPerFrame;
            for (unsigned long m = 0; m < All.size(); ++m) {
                if (T()) return;
                const Creature& c = All[m];
                if (c.hit_hero || (c.special_hero && !c.heal) || c.near_follower) {
                    for (unsigned long n = 0; n < All.size(); ++n)
                        if (Near(c, All[n]) && All[n].guard) { if (T()) return; Log("guard->hero"); }
                }
                if (c.hit_fighter || (c.special_hero && !c.heal && c.hit_fighter)) {
                    for (unsigned long n = 0; n < All.size(); ++n)
                        if (Near(c, All[n]) && All[n].guard) { if (T()) return; Log("guard->fighter"); }
                }
            }
            UpdateLiveEnemies();
            Spawned += DownPerFrame; // the world: guards taken off the counter this frame
        }
        if (T()) return;
        MissionSucceeded = true;
        Log("CS_OUTRO");
        for (long b = 0; b < Beds; ++b) { if (T()) return; Log("bed_on"); }
        if (T()) return;
        Log("completed");
        if (SickChildActive) { if (T()) return; Log("sickchild"); }
    }
};
static Creature C(int id, bool g, bool s, bool hh, bool sp, bool heal, bool nf, bool hf, int x)
{ Creature c = { id, g, s, hh, sp, heal, nf, hf, x }; return c; }
static bool Run(Script& s, const char* want)
{
    g_log[0] = 0; s.Main();
    if (std::strcmp(g_log, want) != 0) { std::printf("got  %s\nwant %s\n", g_log, want); return false; }
    return true;
}
int main()
{
    bool ok = true;
    {   // full run: north intro, one shopkeeper (index 1, the j=0 search reads All[0], a guard: no erase), hero hits a villager
        Script s; s.LoadedAt = 2; s.TerminateAt = 1000; s.Beds = 2; s.SickChildActive = true; s.DownPerFrame = 25;
        s.All.push_back(C(1, true, false, false, false, false, false, false, 0));
        s.All.push_back(C(2, false, true, true, false, false, false, false, 3));
        s.All.push_back(C(3, false, false, false, true, true, false, true, 40));
        ok &= Run(s, "bed_off;bed_off;CS_NORTH;start_screen;faction;faction;fake;stategroups_off;faction;stategroups_off;"
                     "guard->hero;CS_OUTRO;bed_on;bed_on;completed;sickchild;");
        ok &= s.MissionSucceeded && s.QuestStartScreened;
    }
    {   // shopkeeper first: the j=0 search finds it and erases it; quest screen already shown; south; fighter hit near a guard
        Script s; s.LoadedAt = 0; s.TerminateAt = 1000; s.Beds = 1; s.South = true; s.QuestStartScreened = true; s.DownPerFrame = 25;
        s.All.push_back(C(7, false, true, false, false, false, false, false, 0));
        s.All.push_back(C(8, true, false, false, false, false, false, false, 50));
        s.All.push_back(C(9, false, false, false, false, false, false, true, 55));
        ok &= Run(s, "bed_off;CS_SOUTH;faction;fake;stategroups_off;faction;faction;stategroups_off;erase;"
                     "guard->fighter;CS_OUTRO;bed_on;completed;");
        ok &= s.All.size() == 2 && s.All[0].id == 8;
    }
    {   // a heal-spell hit is not an attack; terminate during the bed loop returns at once
        Script s; s.LoadedAt = 0; s.TerminateAt = 1000; s.Beds = 0; s.DownPerFrame = 13;
        s.All.push_back(C(1, true, false, false, true, true, false, false, 0));
        ok &= Run(s, "CS_NORTH;start_screen;faction;CS_OUTRO;completed;");
        Script t; t.LoadedAt = 5; t.TerminateAt = 3; t.Beds = 3;
        ok &= Run(t, "") && !t.MissionSucceeded;
    }
    std::printf(ok ? "00df6010_TEST PASS\n" : "00df6010_TEST FAIL\n");
    return ok ? 0 : 1;
}