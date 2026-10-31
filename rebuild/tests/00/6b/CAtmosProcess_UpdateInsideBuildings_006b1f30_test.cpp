// Behaviour test for CAtmosProcess::UpdateInsideBuildings (0x006B1F30): the same control flow over a host model.
#include <cstdio>


class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public: long X0, Y0, X1, Y1; };
struct RegionDef { long refs; int type; float level; };
struct Building { C2DBoxI inside; int type; float level; };

static long g_frame;
static C3DVector g_camera;
static RegionDef* g_regionDef;
static Building g_buildings[4]; static int g_buildingCount;
static C2DBoxI g_lastArea;
static int g_fadeCount; static unsigned long g_fadeGroups[4]; static float g_fadeVolumes[4];
static long g_themeDefs; static int g_themeGets; static int g_applyCount;
static const float kGain = 0.25f;

struct Atmos {
    long InsideBuilding; int ReverbType; float ReverbLevel;
    void Apply() { ++g_applyCount; }
    void Fade(unsigned long g, float v) { g_fadeGroups[g_fadeCount] = g; g_fadeVolumes[g_fadeCount] = v; ++g_fadeCount; }
    void Update()
    {
        int reverbType = ReverbType; float reverbLevel = ReverbLevel;
        RegionDef* def = g_regionDef;
        if (def) { ++def->refs; reverbType = def->type; reverbLevel = def->level; }
        long wasInside = InsideBuilding;
        bool checked = false;
        if (g_frame % 4 == 0) {
            checked = true;
            C3DVector pos = g_camera;
            long x = (long)pos.X, y = (long)pos.Y;
            C2DBoxI area = { x - 3, y - 3, x + 3, y + 3 };
            g_lastArea = area;
            long inside = 0;
            for (int i = 0; i < g_buildingCount; ++i) {
                const Building& b = g_buildings[i];
                if (pos.X >= b.inside.X0 && pos.X <= b.inside.X1 && pos.Y >= b.inside.Y0 && pos.Y <= b.inside.Y1) {
                    reverbType = b.type; reverbLevel = b.level; inside = 1; break;
                }
            }
            InsideBuilding = inside;
        }
        if (wasInside != InsideBuilding) {
            float gain = kGain;
            if (InsideBuilding != 0) { Fade(0x800, 1.0f); Fade(0x1000, 0.0f); Fade(0x2000, 1.0f); Fade(0x4000, gain); }
            else { Fade(0x1000, 1.0f); Fade(0x800, 0.0f); Fade(0x4000, 1.0f); Fade(0x2000, gain); }
            long count = g_themeDefs;
            for (long i = 1; i < count; ++i) ++g_themeGets;
        }
        if (checked && (reverbType != ReverbType || reverbLevel != ReverbLevel)) {
            ReverbType = reverbType; ReverbLevel = reverbLevel; Apply();
        }
        if (def) --def->refs;
    }
};

int main()
{
    int ok = 1;
    RegionDef region = { 1, 3, 0.5f };
    g_regionDef = &region;
    Building house = { { 10, 10, 20, 20 }, 7, 0.9f };
    g_buildings[g_buildingCount++] = house;
    g_themeDefs = 5;
    Atmos a = { 0, 0, 0.0f };
    g_frame = 1; g_camera.X = 15.5f; g_camera.Y = 12.0f;            // not a check frame: nothing but the def refcount
    a.Update();
    if (a.InsideBuilding != 0 || g_applyCount != 0 || g_fadeCount != 0 || region.refs != 1) ok = 0;
    g_frame = 4; a.Update();                                         // inside the house: fades + theme loop + apply house reverb
    if (a.InsideBuilding != 1 || g_fadeCount != 4 || g_fadeGroups[0] != 0x800 || g_fadeVolumes[3] != kGain) ok = 0;
    if (g_themeGets != 4 || g_applyCount != 1 || a.ReverbType != 7 || a.ReverbLevel != 0.9f) ok = 0;
    if (g_lastArea.X0 != 12 || g_lastArea.Y0 != 9 || g_lastArea.X1 != 18 || g_lastArea.Y1 != 15) ok = 0;
    g_fadeCount = 0; g_frame = 8; g_camera.X = 40.0f; a.Update();    // outside again: region reverb comes back
    if (a.InsideBuilding != 0 || g_fadeGroups[0] != 0x1000 || g_fadeGroups[3] != 0x2000 || g_applyCount != 2) ok = 0;
    if (a.ReverbType != 3 || a.ReverbLevel != 0.5f || region.refs != 1) ok = 0;
    g_fadeCount = 0; g_frame = 12; a.Update();                       // unchanged: no fades, no apply
    if (g_fadeCount != 0 || g_applyCount != 2) ok = 0;
    if (!ok) { std::printf("006b1f30_TEST FAIL\n"); return 1; }
    std::printf("006b1f30_TEST PASS\n");
    return 0;
}