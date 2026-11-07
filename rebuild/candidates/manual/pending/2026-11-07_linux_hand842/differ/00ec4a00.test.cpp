// Behaviour test for 0x00ec4a00 NScript::CV_RockTrollFirstEncounterScript::CRTFE_RockTroll::Main: spawn WatchForRockTrollKilled on
// Me, take scripted control, give the two rewards once, play the exhume cut scene once (HERO / TROLL cast, paused movie, spawn
// WatchForRockTrollHit), play SPECIAL_BOAST then SPECIAL_IDLE and wait, release, face and target the hero, then idle until the
// thread terminates.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>

static int g_frame, g_endFrame, g_starts, g_releases, g_items, g_cutscenes, g_movies, g_pauses, g_anims, g_look, g_target;
static int g_busyUntil;
static std::string g_spawned[4]; static int g_nspawned; static std::string g_anim[4]; static std::string g_cast;
static bool g_valid;

struct CParent { bool AddedItemsToRockTroll; bool PlayedExhumeCutScene; };
struct CSeh {
    bool IsValid() const { return g_valid; }
    void Release() { ++g_releases; g_valid = false; }
    void PlayAnimation(const char* a) { g_anim[g_anims++ & 3] = a; }
    bool IsPerformingScriptTask() const { return g_frame < g_busyUntil; }
};
static bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
static void NewScriptFrame() { ++g_frame; }
static bool StartScriptingEntity(int, CSeh&) { ++g_starts; g_valid = true; return g_starts % 2 == 0; }
static void Spawn(const std::string& name) { g_spawned[g_nspawned++ & 3] = name; }

static void Main(CParent& ParentClass)
{
    Spawn(std::string("ParentClass.") + "WatchForRockTrollKilled");
    CSeh seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!StartScriptingEntity(1, seh_me)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (!ParentClass.AddedItemsToRockTroll) {
        if (IsActiveThreadTerminating()) return;
        g_items += 2;
        ParentClass.AddedItemsToRockTroll = true;
    }
    if (!ParentClass.PlayedExhumeCutScene) {
        if (IsActiveThreadTerminating()) return;
        CSeh seh_hero;
        if (seh_hero.IsValid()) seh_hero.Release();
        while (!StartScriptingEntity(0, seh_hero)) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        std::map<std::string, int> mymap; mymap["HERO"] = 0; mymap["TROLL"] = 1;
        ++g_movies; ++g_pauses;
        g_cast = mymap.begin()->first + "," + (++mymap.begin())->first; ++g_cutscenes;
        Spawn(std::string("ParentClass.") + "WatchForRockTrollHit");
        ParentClass.PlayedExhumeCutScene = true;
        --g_pauses;
    }
    seh_me.PlayAnimation("SPECIAL_BOAST");
    seh_me.PlayAnimation("SPECIAL_IDLE");
    while (seh_me.IsPerformingScriptTask()) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
    if (IsActiveThreadTerminating()) return;
    if (seh_me.IsValid()) seh_me.Release();
    ++g_look; ++g_target;
    for (;;) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
}

int main()
{
    bool ok = true;
    CParent p = { false, false };
    g_endFrame = 50; g_busyUntil = 10;
    Main(p);
    ok = ok && g_nspawned == 2 && g_spawned[0] == "ParentClass.WatchForRockTrollKilled" && g_spawned[1] == "ParentClass.WatchForRockTrollHit";
    ok = ok && g_items == 2 && p.AddedItemsToRockTroll && p.PlayedExhumeCutScene && g_cutscenes == 1 && g_cast == "HERO,TROLL";
    ok = ok && g_pauses == 0 && g_anims == 2 && g_anim[0] == "SPECIAL_BOAST" && g_anim[1] == "SPECIAL_IDLE";
    ok = ok && g_look == 1 && g_target == 1 && g_frame == 50 && !g_valid;
    // second run: rewards and cut scene already done
    g_frame = 0; g_nspawned = 0; g_items = 0; g_cutscenes = 0; g_anims = 0; g_look = 0; g_starts = 0;
    Main(p);
    ok = ok && g_nspawned == 1 && g_items == 0 && g_cutscenes == 0 && g_look == 1;
    // terminated while waiting for control: nothing else happens
    g_frame = 0; g_endFrame = 0; g_nspawned = 0; g_look = 0; g_anims = 0; g_starts = 1;
    p.AddedItemsToRockTroll = false;
    Main(p);
    ok = ok && g_nspawned == 1 && !p.AddedItemsToRockTroll && g_anims == 0 && g_look == 0;
    std::printf(ok ? "00ec4a00_TEST PASS\n" : "00ec4a00_TEST FAIL\n");
    return ok ? 0 : 1;
}
