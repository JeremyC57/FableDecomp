// Behaviour test for NScript::CQ_BountyHuntScript::Main (0x00D13580): the bounty hunt quest main.
// Modelled decisions (the row's own code needs the running game, as for the landed script-Main tests of this lane):
//  * main loop: each frame (until QuestSucceeded or QuestFailed) when the hero's region (regionName) is loaded, every TeleportInfo
//    entry whose teleportTo matches it is re-created: GreatwoodEntrance needs FishComplete, GreatwoodLake needs FishComplete and
//    Deputy1Killed, Fisherman needs nothing. CCharString == text semantics: an EMPTY (null) string equals "" only, so a cleared
//    entry never matches. A re-created entry is cleared (defName / seed / health / script / marker / teleportTo).
//  * the creature is created at the marker with the entry's def / script name, seed set, health moved to the stored value
//    (ModifyThingHealth(health - current)), information off, attached to Q_BountyHunt.
//  * end: failed -> FAILED_INTRO when a villager was killed before the villager intro, else HOSTAGE1 / HOSTAGE2 text, else no text;
//    succeeded -> BountyHuntWithinTimeLimit when the boast time limit is active and its timer is still positive, morality, completed.
// The mutants "GreatwoodLake without Deputy1Killed", "entry not cleared" and "time limit ignores the timer" each fail.
#include <cstdio>
#include <cstring>
#include <string>
static std::string g_log;
static void Log(const std::string& s) { g_log += s; g_log += ";"; }
struct Str {                       // CCharString model: null data = empty string
    const char* P;
    bool operator==(const char* text) const { if (!P) return std::strcmp(text, "") == 0; return std::strcmp(P, text) == 0; }
};
struct TeleportData { Str defName; unsigned long appearanceSeed; float health; Str scriptName; Str teleportMkr; Str teleportTo; };
struct Model {
    int Mutant;
    bool FishComplete, Deputy1Killed;
    TeleportData Info[4]; int Count;
    float CurrentHealth;
    bool Matches(const Str& region, const Str& to) const
    {
        bool lakeNeedsDeputy = Mutant != 1;
        return (region == "GreatwoodEntrance" && to == "GreatwoodEntrance" && FishComplete) ||
               (region == "GreatwoodLake" && to == "GreatwoodLake" && FishComplete && (Deputy1Killed || !lakeNeedsDeputy)) ||
               (region == "Fisherman" && to == "Fisherman");
    }
    void Frame(const Str& region, bool loaded)
    {
        if (!loaded) return;
        for (int i = 0; i < Count; ++i) {
            TeleportData t = Info[i];
            if (!Matches(region, t.teleportTo)) continue;
            char buf[160];
            std::sprintf(buf, "create %s at %s as %s seed %lu dh %.0f", t.defName.P ? t.defName.P : "",
                         t.teleportMkr.P ? t.teleportMkr.P : "", t.scriptName.P ? t.scriptName.P : "", t.appearanceSeed,
                         (double)(t.health - CurrentHealth));
            Log(buf);
            if (Mutant == 2) continue;
            Info[i].defName.P = 0; Info[i].appearanceSeed = 0; Info[i].health = 0;
            Info[i].scriptName.P = 0; Info[i].teleportMkr.P = 0; Info[i].teleportTo.P = 0;
        }
    }
};
static const char* EndText(bool failed, bool succeeded, bool villagerKilled, bool villagerIntroDone, bool h1, bool h2,
                           bool limitActive, long timer, int mutant, bool* withinLimit)
{
    *withinLimit = false;
    if (failed) {
        if (villagerKilled && !villagerIntroDone) return "FAILED_INTRO";
        if (h1) return "FAILED_HOSTAGE1";
        if (h2) return "FAILED_HOSTAGE2";
        return "FAILED";
    }
    if (succeeded) {
        if (limitActive && (mutant == 3 || timer > 0)) *withinLimit = true;
        return "COMPLETED";
    }
    return "";
}
static std::string Run(int mutant)
{
    g_log = "";
    Model m; std::memset(&m, 0, sizeof(m)); m.Mutant = mutant; m.CurrentHealth = 40.0f;
    TeleportData a = { {"CreatureBandit"}, 7, 100.0f, {"BH_Grunt1"}, {"M_Entrance1"}, {"GreatwoodEntrance"} };
    TeleportData b = { {"CreatureBanditDeputy"}, 9, 300.0f, {"BH_Deputy2"}, {"M_Lake1"}, {"GreatwoodLake"} };
    TeleportData c = { {"CreatureBandit"}, 3, 80.0f, {"BH_Grunt2"}, {"M_Fish1"}, {"Fisherman"} };
    TeleportData d = { {0}, 0, 0.0f, {0}, {0}, {0} };      // cleared entry: never matches
    m.Info[0] = a; m.Info[1] = b; m.Info[2] = c; m.Info[3] = d; m.Count = 4;
    Str entrance = {"GreatwoodEntrance"}, lake = {"GreatwoodLake"}, fisher = {"Fisherman"}, none = {0};
    m.Frame(fisher, false);                  // region not loaded: nothing
    m.Frame(entrance, true);                 // FishComplete false: nothing
    m.Frame(fisher, true);                   // Fisherman: re-created
    m.Frame(fisher, true);                   // cleared: not again
    m.FishComplete = true;
    m.Frame(lake, true);                     // Deputy1 alive: nothing
    m.Frame(none, true);                     // empty region name only matches cleared entries' tests -> nothing
    m.Frame(entrance, true);                 // entrance grunt
    m.Deputy1Killed = true;
    m.Frame(lake, true);                     // deputy 2 re-created
    bool w = false;
    Log(EndText(true, false, true, false, true, false, false, 0, mutant, &w));
    Log(EndText(true, false, true, true, true, true, false, 0, mutant, &w));
    Log(EndText(true, false, false, false, false, true, false, 0, mutant, &w));
    Log(EndText(true, false, false, false, false, false, false, 0, mutant, &w));
    Log(EndText(false, true, false, false, false, false, true, 0, mutant, &w)); Log(w ? "limit" : "nolimit");
    Log(EndText(false, true, false, false, false, false, true, 25, mutant, &w)); Log(w ? "limit" : "nolimit");
    return g_log;
}
int main()
{
    const std::string want =
        "create CreatureBandit at M_Fish1 as BH_Grunt2 seed 3 dh 40;"
        "create CreatureBandit at M_Entrance1 as BH_Grunt1 seed 7 dh 60;"
        "create CreatureBanditDeputy at M_Lake1 as BH_Deputy2 seed 9 dh 260;"
        "FAILED_INTRO;FAILED_HOSTAGE1;FAILED_HOSTAGE2;FAILED;COMPLETED;nolimit;COMPLETED;limit;";
    std::string got = Run(0);
    bool ok = got == want;
    for (int mut = 1; mut <= 3; ++mut)
        if (Run(mut) == want) { ok = false; std::printf("mutant %d not detected\n", mut); }
    if (!ok) std::printf("got  %s\nwant %s\n", got.c_str(), want.c_str());
    std::printf(ok ? "00d13580_TEST PASS\n" : "00d13580_TEST FAIL\n");
    return ok ? 0 : 1;
}
