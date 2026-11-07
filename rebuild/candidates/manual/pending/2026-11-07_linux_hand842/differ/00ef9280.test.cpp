// Behaviour test for 0x00ef9280 NScript::Cjoss_test_scriptsScript::Main: activate guy_script and bandit_level_cutscene; per frame
// look up fireh, then summoner, then take the summoner over and fire its lightning orb at fireh whenever it is idle; jim's hit
// queries once jim is found; the hero's flourish queries; bob gets the combo multiplier, my_generator the depleted query; pinky
// and perky are looked up; the dan_bandit things are set on the hero once (only while none have been found yet).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

static int g_frame, g_endFrame, g_lookups[8], g_orbs, g_jimQueries, g_heroQueries, g_combo, g_depleted, g_targets, g_banditScans;
static int g_starts, g_quests; static bool g_valid, g_busy; static int g_banditFrame;
static const char* g_names[8] = { "fireh", "summoner", "jim", "bob", "my_generator", "pinky", "perky", "dan_bandit" };
struct CScriptThing { int Id; CScriptThing() : Id(0) {} bool IsAlive() const { return Id != 0; } };
static bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
static CScriptThing GetThingWithScriptName(const char* n)
{ CScriptThing t; for (int i = 0; i < 8; ++i) if (!std::strcmp(n, g_names[i])) { ++g_lookups[i]; t.Id = (g_frame >= i) ? i + 1 : 0; } return t; }
static void GetAllThingsWithScriptName(std::vector<CScriptThing>& out)
{ ++g_banditScans; if (g_frame >= g_banditFrame) { CScriptThing t; t.Id = 9; out.push_back(t); out.push_back(t); } }

static void Main()
{
    CScriptThing bob, jim, my_generator, pinky, perky, summoner, fireh;
    bool bandits_attacking = false;
    ++g_quests; ++g_quests;
    for (; !IsActiveThreadTerminating(); ++g_frame) {
        if (!fireh.IsAlive()) { if (IsActiveThreadTerminating()) return; fireh = GetThingWithScriptName("fireh"); }
        else {
            if (IsActiveThreadTerminating()) return;
            if (!summoner.IsAlive()) { if (IsActiveThreadTerminating()) return; summoner = GetThingWithScriptName("summoner"); }
            else if (!g_valid) { if (IsActiveThreadTerminating()) return; ++g_starts; g_valid = true; }
            else { if (IsActiveThreadTerminating()) return; if (!g_busy) { if (IsActiveThreadTerminating()) return; ++g_orbs; } }
        }
        if (!jim.IsAlive()) { if (IsActiveThreadTerminating()) return; jim = GetThingWithScriptName("jim"); }
        else { if (IsActiveThreadTerminating()) return; g_jimQueries += 2; }
        g_heroQueries += 2;
        if (!bob.IsAlive()) { if (IsActiveThreadTerminating()) return; bob = GetThingWithScriptName("bob"); }
        else { if (IsActiveThreadTerminating()) return; ++g_combo; }
        if (!my_generator.IsAlive()) { if (IsActiveThreadTerminating()) return; my_generator = GetThingWithScriptName("my_generator"); }
        else { if (IsActiveThreadTerminating()) return; ++g_depleted; }
        if (!pinky.IsAlive()) { if (IsActiveThreadTerminating()) return; pinky = GetThingWithScriptName("pinky"); }
        if (!perky.IsAlive()) { if (IsActiveThreadTerminating()) return; perky = GetThingWithScriptName("perky"); }
        if (pinky.IsAlive() && perky.IsAlive()) { if (IsActiveThreadTerminating()) return; }
        if (!bandits_attacking) {
            if (IsActiveThreadTerminating()) return;
            std::vector<CScriptThing> bandits;
            GetAllThingsWithScriptName(bandits);
            for (unsigned long i = 0; i < bandits.size(); i++) { bandits_attacking = true; ++g_targets; }
        }
    }
}

int main()
{
    g_endFrame = 10; g_banditFrame = 3;
    Main();
    bool ok = g_quests == 2 && g_frame == 10;
    ok = ok && g_lookups[0] == 1 && g_lookups[1] == 1;                 // fireh found frame 0, summoner frame 1
    ok = ok && g_starts == 1 && g_orbs == 7;                           // scripted frame 2, orb frames 3..9
    ok = ok && g_lookups[2] == 3 && g_jimQueries == 2 * 7;             // jim found frame 2
    ok = ok && g_heroQueries == 20 && g_lookups[3] == 4 && g_combo == 6 && g_lookups[4] == 5 && g_depleted == 5;
    ok = ok && g_lookups[5] == 6 && g_lookups[6] == 7;
    ok = ok && g_banditScans == 4 && g_targets == 2;                   // scans frames 0..3, two bandits set once
    std::printf(ok ? "00ef9280_TEST PASS\n" : "00ef9280_TEST FAIL\n");
    return ok ? 0 : 1;
}
