// Behaviour test for 0x00CF0790 NScript::CQ_ArenaHoldingScriptScript::Main: region wait, six bindings, door unusable + persistent, timer bracket
#include <cstdio>
#include <cstring>
static int gTimers = 0, gFrames = 0, gBindings = 0, gUsable = -1, gPersistent = -1, gPost = 0;
static const char* gNames[8];
class CScriptThing { public: const char* Name; };
class CGame { public:
    int RegionAfter;
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "ArenaExterior") == 0 && gFrames >= RegionAfter; }
    void NewScriptFrame() { ++gFrames; }
    void PostAddScriptedEntities() { ++gPost; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.Name = n; return t; }
    void SetThingAsUsable(const CScriptThing& t, bool u) { if (std::strcmp(t.Name, "ArenaExtToHallOfHeroes") == 0) gUsable = u; }
    void SetThingPersistent(const CScriptThing& t, bool p) { if (std::strcmp(t.Name, "ArenaExtToHallOfHeroes") == 0) gPersistent = p; }
};
class CTimer { public: CTimer() { ++gTimers; } ~CTimer() { --gTimers; } };
struct CScript {
    CGame* Game; int TerminateAt;
    bool IsActiveThreadTerminating() { return gFrames >= TerminateAt; }
    void AddEntityScriptBinding(const char* name, unsigned long flags) { if (flags == 1) gNames[gBindings++] = name; }
    void PostAddScriptedEntities() { ++gPost; }
    void Main()
    {
        CTimer timer;
        while (!Game->IsRegionLoaded("ArenaExterior")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        AddEntityScriptBinding("EntranceGuard", 1);
        AddEntityScriptBinding("EntranceGuard2", 1);
        AddEntityScriptBinding("OakvaleFan", 1);
        AddEntityScriptBinding("BowerstoneFan", 1);
        AddEntityScriptBinding("HookCoastFan", 1);
        AddEntityScriptBinding("FanMarker", 1);
        PostAddScriptedEntities();
        Game->PostAddScriptedEntities();
        CScriptThing door = Game->GetThingWithScriptName("ArenaExtToHallOfHeroes");
        Game->SetThingAsUsable(door, false);
        Game->SetThingPersistent(door, true);
    }
};
int main()
{
    CGame g; g.RegionAfter = 3; CScript s; s.Game = &g; s.TerminateAt = 100;
    s.Main();
    if (gFrames != 3 || gBindings != 6 || std::strcmp(gNames[5], "FanMarker") != 0 || gPost != 2 || gUsable != 0 || gPersistent != 1 || gTimers != 0) { std::printf("00cf0790_TEST FAIL\n"); return 1; }
    gFrames = 0; gBindings = 0; gUsable = -1; g.RegionAfter = 50; s.TerminateAt = 2;
    s.Main();
    if (gFrames != 2 || gBindings != 0 || gUsable != -1 || gTimers != 0) { std::printf("00cf0790_TEST FAIL\n"); return 1; }
    std::printf("00cf0790_TEST PASS\n");
    return 0;
}