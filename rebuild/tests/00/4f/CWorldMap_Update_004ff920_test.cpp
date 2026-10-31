// Behaviour test for CWorldMap::Update (0x004FF920): run the level loader, update each used and loaded map, update the
// destroyed theme areas, run the set-moving callback over every height-changed area (then clear the list), and switch
// the passability overlay on/off only when the debug toggle and the shown flag disagree.
#include <cstdio>

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class CThingPhysical { public: long X, Y; bool Moving; };

static CThingPhysical g_things[4] = { {1, 1, false}, {5, 5, false}, {9, 9, false}, {20, 20, false} };
static bool CThingPhysical_SetAsMoving(CThingPhysical& thing) { thing.Moving = true; return true; }
typedef bool (*ThingCallback)(CThingPhysical&);
static bool ForEachInRoughArea(const C2DBoxI& area, const ThingCallback& callback)
{
    bool any = false;
    for (int i = 0; i < 4; ++i)
        if (g_things[i].X >= area.TLX && g_things[i].X <= area.BRX && g_things[i].Y >= area.TLY && g_things[i].Y <= area.BRY)
            any = callback(g_things[i]) || any;
    return any;
}

class CMap { public: int Updates; void Update() { ++Updates; } };
class CMapInfo { public: CMap* PMap; bool Used; bool Loaded; };
static bool g_bShowPassability = false;

class CWorldMap { public:
    int       LoaderRuns;
    int       ThemeUpdates;
    CMapInfo  Maps[3];
    unsigned  MapCount;
    C2DBoxI   Areas[4];
    C2DBoxI*  AreasFinish;
    bool      PassabilityBeingShown;
    int       OverlayCalls;
    bool      OverlayState;
    bool IsMapLoaded(unsigned long map) const { return Maps[map].Loaded; }
    void ShowPassabilityOverlay(bool show) { ++OverlayCalls; OverlayState = show; }
    void Update()
    {
        ++LoaderRuns;
        for (unsigned long map = 0; map < MapCount; ++map) {
            if (Maps[map].Used && IsMapLoaded(map))
                Maps[map].PMap->Update();
        }
        ++ThemeUpdates;
        for (C2DBoxI* area = Areas; area != AreasFinish; ++area)
            ForEachInRoughArea(*area, &CThingPhysical_SetAsMoving);
        AreasFinish = Areas;                    // clear()
        if (g_bShowPassability) {
            if (!PassabilityBeingShown) {
                ShowPassabilityOverlay(true);
                PassabilityBeingShown = true;
            }
        } else if (PassabilityBeingShown) {
            ShowPassabilityOverlay(false);
            PassabilityBeingShown = false;
        }
    }
};

int main()
{
    CMap maps[3] = { {0}, {0}, {0} };
    CWorldMap w;
    w.LoaderRuns = 0; w.ThemeUpdates = 0; w.MapCount = 3;
    w.Maps[0].PMap = &maps[0]; w.Maps[0].Used = true;  w.Maps[0].Loaded = true;
    w.Maps[1].PMap = &maps[1]; w.Maps[1].Used = false; w.Maps[1].Loaded = true;
    w.Maps[2].PMap = &maps[2]; w.Maps[2].Used = true;  w.Maps[2].Loaded = false;
    C2DBoxI a0 = { 0, 0, 6, 6 }, a1 = { 8, 8, 10, 10 };
    w.Areas[0] = a0; w.Areas[1] = a1; w.AreasFinish = w.Areas + 2;
    w.PassabilityBeingShown = false; w.OverlayCalls = 0; w.OverlayState = false;

    w.Update();
    if (w.LoaderRuns != 1 || w.ThemeUpdates != 1 || maps[0].Updates != 1 || maps[1].Updates != 0 || maps[2].Updates != 0 ||
        !g_things[0].Moving || !g_things[1].Moving || !g_things[2].Moving || g_things[3].Moving ||
        w.AreasFinish != w.Areas || w.OverlayCalls != 0) {
        std::printf("004ff920_TEST FAIL\n");
        return 1;
    }
    g_bShowPassability = true;
    w.Update();
    w.Update();                                 // already shown: no second call
    if (w.OverlayCalls != 1 || !w.OverlayState || !w.PassabilityBeingShown || maps[0].Updates != 3) {
        std::printf("004ff920_TEST FAIL\n");
        return 1;
    }
    g_bShowPassability = false;
    w.Update();
    if (w.OverlayCalls != 2 || w.OverlayState || w.PassabilityBeingShown) {
        std::printf("004ff920_TEST FAIL\n");
        return 1;
    }
    std::printf("004ff920_TEST PASS\n");
    return 0;
}